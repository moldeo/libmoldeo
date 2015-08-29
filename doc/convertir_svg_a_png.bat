"C:\Program Files (x86)\Inkscape\inkscape.com" --usage
for %%f in (*.svg) do (
	"C:\Program Files (x86)\Inkscape\inkscape.com" %%~nf.svg --export-png=%%~nf.png --export-width=500 --export-background=#FFFFFF
)
pause