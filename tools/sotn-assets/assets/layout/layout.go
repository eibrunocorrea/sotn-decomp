package layout

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
	"github.com/xeeynamo/sotn-decomp/tools/sotn-assets/datarange"
	"github.com/xeeynamo/sotn-decomp/tools/sotn-assets/psx"
	"github.com/xeeynamo/sotn-decomp/tools/sotn-assets/sotn"
	"github.com/xeeynamo/sotn-decomp/tools/sotn-assets/util"
	"hash/fnv"
	"io"
	"os"
	"path/filepath"
	"sort"
	"strconv"
	"strings"
)

type layoutEntry struct {
	X       int16  `json:"x"`
	Y       int16  `json:"y"`
	ID      string `json:"id"`
	Flags   uint8  `json:"flags"` // TODO properly de-serialize this
	Slot    uint8  `json:"slot"`
	SpawnID uint8  `json:"spawnId"`
	Params  uint16 `json:"params"`
	YOrder  *int   `json:"yOrder,omitempty"`
}

// raw words embedded between two layout blocks but not referenced by any
// layout table (e.g. NO0 has an orphan spawn cluster inside the Y-ordered
// copy); preserved so the build stays byte-exact
type layoutGap struct {
	Copy       string   `json:"copy"`       // "x" or "y"
	AfterBlock int      `json:"afterBlock"` // address-ordered block index the gap follows
	Words      []uint16 `json:"words"`
}

type layouts struct {
	Entities [][]layoutEntry `json:"entities"`
	Indices  []int           `json:"indices"`
	Gaps     []layoutGap     `json:"gaps,omitempty"`
}

// Utility function that finds the index of the given value in the given list.
// Returns -1 (invalid index) if value is not in list.
func indexOf(searchList []int, searchVal int) int {

	for i, value := range searchList {
		if value == searchVal {
			return i
		}
	}
	return -1
}

func fetchEntityIDsFromHeaderFile(overlay string) (map[int]string, error) {
	var path = "src/st"
	if strings.HasPrefix(overlay, "bo") || strings.HasPrefix(overlay, "rbo") ||
		overlay == "mar" {
		path = "src/boss"
	}
	path += "/" + overlay
	return sotn.FetchEnumWithMin(path, overlay, "EntityID", 0x100)
}

func readEntityLayoutEntry(file io.ReadSeeker, ovlName string) (layoutEntry, error) {
	entityIDs, _ := fetchEntityIDsFromHeaderFile(ovlName)

	bs := make([]byte, 10)
	if _, err := io.ReadFull(file, bs); err != nil {
		return layoutEntry{}, err
	}

	var entityIDStr string
	id := int(bs[4])
	entityIDStr = entityIDs[id]
	if entityIDStr == "" {
		entityIDStr = fmt.Sprintf("0x%02X", id)
	}

	return layoutEntry{
		X:       int16(binary.LittleEndian.Uint16(bs[0:2])),
		Y:       int16(binary.LittleEndian.Uint16(bs[2:4])),
		ID:      entityIDStr,
		Flags:   bs[5],
		Slot:    bs[6],
		SpawnID: bs[7],
		Params:  binary.LittleEndian.Uint16(bs[8:10]),
	}, nil
}

// the Y-ordered entries list has a different order than the X-ordered one. The order cannot consistently get
// restored by just sorting entries by Y as usually entries with the same Y results swapped.
// This algorithm will fill the optional field YOrder, only useful to restore the original order.
func hydrateYOrderFields(x layouts, y layouts) error {
	if len(x.Indices) != len(y.Indices) {
		return fmt.Errorf("number of X and Y layout indices do not match")
	}
	if len(x.Entities) != len(y.Entities) {
		return fmt.Errorf("number of X and Y layout entries do not match")
	}

	populateYOrderField := func(xEntries []layoutEntry, yEntries []layoutEntry) {
		yIndexMap := make(map[layoutEntry]int, len(yEntries))
		for i, e := range yEntries {
			yIndexMap[e] = i
		}
		for i := 0; i < len(xEntries); i++ {
			if yOrder, found := yIndexMap[xEntries[i]]; found {
				xEntries[i].YOrder = &yOrder
			}
		}
	}

	for i := 0; i < len(x.Entities); i++ {
		xList := x.Entities[i]
		yList := y.Entities[i]
		if len(xList) != len(yList) {
			return fmt.Errorf("number of X and Y entries do not match")
		}
		populateYOrderField(xList, yList)
	}
	return nil
}

func readEntityLayout(r io.ReadSeeker, ovlName string, off, baseAddr psx.Addr, count int, isX bool) (layouts, []datarange.DataRange, error) {
	if err := off.MoveFile(r, baseAddr); err != nil {
		return layouts{}, nil, err
	}

	// there are two copies of the layout, one ordered by X and the other one ordered by Y
	// we will only read the first one, which is ordered by Y
	blockOffsets := make([]psx.Addr, count)
	if err := binary.Read(r, binary.LittleEndian, blockOffsets); err != nil {
		return layouts{}, nil, err
	}

	// the order of each layout entry must be preserved
	pool := map[psx.Addr]int{}
	var blocks [][]layoutEntry
	var xRanges []datarange.DataRange
	for _, blockOffset := range util.SortUniqueOffsets(blockOffsets) {
		if err := blockOffset.MoveFile(r, baseAddr); err != nil {
			return layouts{}, nil, err
		}
		var entries []layoutEntry
		for {
			entry, err := readEntityLayoutEntry(r, ovlName)
			if err != nil {
				return layouts{}, nil, err
			}
			if entry.X == -1 && entry.Y == -1 {
				entries = append(entries, entry)
				break
			}
			entries = append(entries, entry)
		}

		// sanity check on the first entry
		if entries[0].X != -2 || entries[0].Y != -2 {
			err := fmt.Errorf("first layout entry does not mark the beginning of the array: %v", entries[0])
			return layouts{}, nil, err
		}

		pool[blockOffset] = len(blocks)
		blocks = append(blocks, entries)
		xRanges = append(xRanges, datarange.FromAddr(blockOffset, len(entries)*10))
	}
	// the very last entry needs to be aligned by 4
	xRanges[len(xRanges)-1] = xRanges[len(xRanges)-1].Align4()

	l := layouts{Entities: blocks}
	for _, blockOffset := range blockOffsets {
		l.Indices = append(l.Indices, pool[blockOffset])
	}

	endOfArray := off.Sum(count * 4)
	if isX { // we want to do the same thing with the vertically aligned layout
		yLayouts, yRanges, err := readEntityLayout(r, ovlName, endOfArray, baseAddr, count, false)
		if err != nil {
			return layouts{}, nil, fmt.Errorf("readEntityLayout failed on Y: %w", err)
		}
		if err := hydrateYOrderFields(l, yLayouts); err != nil {
			return layouts{}, nil, fmt.Errorf("unable to populate YOrder field: %w", err)
		}
		xMerged, xGaps, err := mergeRangesCapturingGaps(r, baseAddr, xRanges, "x")
		if err != nil {
			return layouts{}, nil, err
		}
		yMerged := yRanges[1]
		defRange := datarange.MergeDataRanges([]datarange.DataRange{datarange.New(off, endOfArray), yRanges[0]})
		l.Gaps = append(xGaps, yLayouts.Gaps...)
		var dataRange datarange.DataRange
		if xMerged.End() != yMerged.Begin() {
			// unreferenced bytes between the X and Y copies: keep them as a
			// tail gap of the last X block so Build reproduces them in place
			words, err := readGapWords(r, baseAddr, xMerged.End(), yMerged.Begin())
			if err != nil {
				return layouts{}, nil, err
			}
			l.Gaps = append(l.Gaps, layoutGap{Copy: "x", AfterBlock: len(xRanges) - 1, Words: words})
			dataRange = datarange.New(xMerged.Begin(), yMerged.End())
		} else {
			dataRange = datarange.MergeDataRanges([]datarange.DataRange{xMerged, yMerged})
		}
		return l, []datarange.DataRange{defRange, dataRange}, nil
	} else {
		merged, gaps, err := mergeRangesCapturingGaps(r, baseAddr, xRanges, "y")
		if err != nil {
			return layouts{}, nil, err
		}
		l.Gaps = gaps
		return l, []datarange.DataRange{datarange.New(off, endOfArray), merged}, nil
	}
}

// reads the raw words of an unreferenced hole [begin, end) inside the layout data
func readGapWords(r io.ReadSeeker, baseAddr, begin, end psx.Addr) ([]uint16, error) {
	gapLen := int(end) - int(begin)
	if gapLen <= 0 || gapLen%2 != 0 {
		return nil, fmt.Errorf("layout blocks overlap or misalign: %s vs %s", begin, end)
	}
	if err := begin.MoveFile(r, baseAddr); err != nil {
		return nil, err
	}
	words := make([]uint16, gapLen/2)
	if err := binary.Read(r, binary.LittleEndian, words); err != nil {
		return nil, err
	}
	return words, nil
}

// like datarange.MergeDataRanges but instead of failing on non-contiguous
// blocks it captures each hole's raw content, tagged with the address-ordered
// block index it follows
func mergeRangesCapturingGaps(r io.ReadSeeker, baseAddr psx.Addr, ranges []datarange.DataRange, copyName string) (datarange.DataRange, []layoutGap, error) {
	var zero datarange.DataRange
	if len(ranges) == 0 {
		return zero, nil, fmt.Errorf("no layout %s ranges, bug?!", copyName)
	}
	sorted := make([]datarange.DataRange, len(ranges))
	copy(sorted, ranges)
	sort.Slice(sorted, func(i, j int) bool { return sorted[i].Begin() < sorted[j].Begin() })
	var gaps []layoutGap
	for i := 0; i < len(sorted)-1; i++ {
		end, next := sorted[i].End(), sorted[i+1].Begin()
		if end == next {
			continue
		}
		words, err := readGapWords(r, baseAddr, end, next)
		if err != nil {
			return zero, nil, fmt.Errorf("layout %s copy: %w", copyName, err)
		}
		gaps = append(gaps, layoutGap{Copy: copyName, AfterBlock: i, Words: words})
	}
	return datarange.New(sorted[0].Begin(), sorted[len(sorted)-1].End()), gaps, nil
}

func buildEntityLayouts(fileName, outputDir, subDir string, ovlName string) error {
	makeSortedBanks := func(banks [][]layoutEntry, sortByX bool) [][]layoutEntry {
		var toSort []layoutEntry
		var less func(i, j int) bool
		if sortByX {
			less = func(i, j int) bool {
				return toSort[i].X < toSort[j].X
			}
		} else {
			less = func(i, j int) bool {
				if toSort[i].Y < toSort[j].Y {
					return true
				}
				if toSort[i].Y > toSort[j].Y {
					return false
				}
				if toSort[i].YOrder != nil && toSort[j].YOrder != nil {
					return *toSort[i].YOrder < *toSort[j].YOrder
				}
				return i < j
			}
		}
		sorting := make([][]layoutEntry, len(banks))
		for i, entries := range banks {
			sorting[i] = make([]layoutEntry, len(entries)-2)
			if len(sorting[i]) > 0 { // do not sort if the list is empty
				copy(sorting[i], entries[1:len(entries)-1]) // do not sort the -2 and -1 entries
				toSort = sorting[i]
				sort.SliceStable(toSort, less)
			}

			// put back the -2 and -1
			sorting[i] = append([]layoutEntry{entries[0]}, sorting[i]...)
			sorting[i] = append(sorting[i], entries[len(entries)-1])
		}
		return sorting
	}
	writeLayoutEntries := func(sb *strings.Builder, el layouts, sortByX bool) error {
		copyName := "y"
		if sortByX {
			copyName = "x"
		}
		banks := makeSortedBanks(el.Entities, sortByX)
		nWritten := 0
		wordsWritten := 0
		for i, entries := range banks {
			// do a sanity check on the entries as we do not want to build something that will cause the game to crash
			if entries[0].X != -2 || entries[0].Y != -2 {
				return fmt.Errorf("layout entity bank %d needs to have a X:-2 and Y:-2 entry at the beginning", i)
			}
			lastEntry := entries[len(entries)-1]
			if lastEntry.X != -1 || lastEntry.Y != -1 {
				return fmt.Errorf("layout entity bank %d needs to have a X:-1 and Y:-1 entry at the end", i)
			}
			roomNum := indexOf(el.Indices, i)
			if roomNum < 0 {
				sb.WriteString(fmt.Sprintf("// Offset %d, No Room Found\n", nWritten))
			} else {
				sb.WriteString(fmt.Sprintf("// Offset %d, Room 0x%02X\n", nWritten, roomNum)) //label each block with offsets
			}
			for _, e := range entries {
				sb.WriteString(fmt.Sprintf("    0x%04X, 0x%04X, %s | 0x%04X, 0x%04X, 0x%04X,\n",
					uint16(e.X), uint16(e.Y), e.ID, int(e.Flags)<<8, int(e.Slot)|(int(e.SpawnID)<<8), e.Params))
			}
			nWritten += len(entries)
			wordsWritten += len(entries) * 5
			for _, gap := range el.Gaps {
				if gap.Copy != copyName || gap.AfterBlock != i {
					continue
				}
				sb.WriteString("// unreferenced raw words (not reachable from the layout tables)\n")
				for j, w := range gap.Words {
					if j%8 == 0 {
						sb.WriteString("   ")
					}
					sb.WriteString(fmt.Sprintf(" 0x%04X,", w))
					if j%8 == 7 || j == len(gap.Words)-1 {
						sb.WriteString("\n")
					}
				}
				wordsWritten += len(gap.Words)
			}
		}
		if !sortByX && wordsWritten%2 != 0 {
			sb.WriteString("    0, // padding\n")
		}
		return nil
	}

	data, err := os.ReadFile(fileName)
	if err != nil {
		return err
	}

	var el layouts
	if err := json.Unmarshal(data, &el); err != nil {
		return err
	}

	h := fnv.New32()
	_, _ = h.Write([]byte(outputDir))
	symbolVariant := strconv.FormatUint(uint64(h.Sum32()), 16)
	symbolName := fmt.Sprintf("entity_layout_%s", symbolVariant)
	// per-copy u16 offsets: unreferenced gaps shift every block after them,
	// and a gap not multiple of 10 bytes breaks LayoutEntity-granular indexing
	makeOffsets := func(copyName string) ([]int, bool) {
		hasGap := false
		out := make([]int, len(el.Entities))
		cur := 0
		for i := 0; i < len(el.Entities); i++ {
			out[i] = cur
			cur += len(el.Entities[i]) * 5
			for _, gap := range el.Gaps {
				if gap.Copy == copyName && gap.AfterBlock == i {
					cur += len(gap.Words)
					hasGap = true
				}
			}
		}
		return out, hasGap
	}

	ovlHeaderLoc := fmt.Sprintf("../%s.h", ovlName)
	if subDir != "" {
		// Look back further if in version specific subdirectory
		ovlHeaderLoc = "../" + ovlHeaderLoc
	}

	laydefFile := strings.Builder{}
	laydefFile.WriteString("#include <stage.h>\n\n")
	laydefFile.WriteString("#include \"common.h\"\n\n")
	laydefFile.WriteString("// clang-format off\n")
	writeLaydef := func(copyName, cName string) {
		offsets, hasGap := makeOffsets(copyName)
		if hasGap {
			// gaps break LayoutEntity-granular indexing: use u16 indices
			laydefFile.WriteString(fmt.Sprintf("extern u16 %s_%s[];\n", symbolName, copyName))
		} else {
			laydefFile.WriteString(fmt.Sprintf("extern LayoutEntity %s_%s[];\n", symbolName, copyName))
		}
		laydefFile.WriteString(fmt.Sprintf("LayoutEntity* %s[] = {\n", cName))
		for _, i := range el.Indices {
			if hasGap {
				laydefFile.WriteString(fmt.Sprintf("    (LayoutEntity*)&%s_%s[%d],\n", symbolName, copyName, offsets[i]))
			} else {
				laydefFile.WriteString(fmt.Sprintf("    &%s_%s[%d],\n", symbolName, copyName, offsets[i]/5))
			}
		}
		laydefFile.WriteString(fmt.Sprintf("};\n"))
	}
	writeLaydef("x", "entityLayoutHorizontal")
	writeLaydef("y", "entityLayoutVertical")

	layoutFile := strings.Builder{}
	layoutFile.WriteString(fmt.Sprintf("#include \"%s\"\n\n", ovlHeaderLoc))
	layoutFile.WriteString("// clang-format off\n")
	layoutFile.WriteString(fmt.Sprintf("u16 %s_x[] = {\n", symbolName))
	if err := writeLayoutEntries(&layoutFile, el, true); err != nil {
		return fmt.Errorf("unable to build X entity layout: %w", err)
	}
	layoutFile.WriteString(fmt.Sprintf("};\n"))
	layoutFile.WriteString(fmt.Sprintf("u16 %s_y[] = {\n", symbolName))
	if err := writeLayoutEntries(&layoutFile, el, false); err != nil {
		return fmt.Errorf("unable to build Y entity layout: %w", err)
	}
	layoutFile.WriteString(fmt.Sprintf("};\n"))

	if err := util.WriteFile(filepath.Join(outputDir, "gen", subDir, "e_layout.c"), []byte(layoutFile.String())); err != nil {
		return err
	}
	return util.WriteFile(filepath.Join(outputDir, "gen", subDir, "e_laydef.c"), []byte(laydefFile.String()))
}
