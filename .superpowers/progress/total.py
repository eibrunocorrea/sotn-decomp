# total.py VERSION : roda o tools/progress.py sem rede e soma os totais por versao
import sys,os,io,contextlib
ver=sys.argv[1]; sys.argv=['progress.py','--version',ver,'--dry-run']
src=open('tools/progress.py').read()
head,main=src.split('if __name__ == "__main__":',1)
main=main.replace('hydrate_previous_metrics(progress, args.version)','')
main=main.split('entry = get_progress_entry(progress)')[0]
g={'__name__':'progress_total','__file__':'tools/progress.py'}
sys.path.insert(0,'tools')
with contextlib.redirect_stdout(io.StringIO()):
    exec(head,g)
    exec('if True:'+main,g)
P=g['progress']
ct=cm=ft=fm=dt=di=0
rows=[]
for k,s in P.items():
    ct+=s.code_total; cm+=s.code_matching; ft+=s.functions_total; fm+=s.functions_matching; dt+=s.data_total; di+=s.data_imported
    rows.append((k,s.code_matching/s.code_total if s.code_total else 1,s.functions_matching,s.functions_total))
print(f'{ver}: codigo {cm/ct*100:.2f}% ({cm}/{ct} bytes) | funcoes {fm/ft*100:.2f}% ({fm}/{ft}) | dados {di/dt*100:.2f}% | overlays {len(P)} | 100%: {sum(1 for r in rows if r[1]>=0.99995)}')
for k,c,f,t in sorted(rows,key=lambda r:r[1])[:12]:
    print(f'   {k:10s} {c*100:6.2f}%  faltam {t-f} fns')
