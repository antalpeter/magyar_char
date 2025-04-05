_CIM_

Ez egy GFX könyvtár változat, amely képes kezelni az összes magyar ékezetes karaktert. Teszi ezt úgy, hogy az Adfruit drawChar függvényét lecseréli 16 bites karaktereket kezelni tudó változatra. A void visszatérés helyett az xa (xAdvanced) értékkel tér vissza. Igy képes a kernelésre is.
A magyar_font.h file-t a **Bodmer** féle fontconvert programmal hoztam létre a megfelelő truetype font file-ból
drawUTF8Text(magyar_szoveg) függvény képes az összes magyar ékezetes karakter megjelenítésére az **Adafruit_ILI9341** -es színes LCD képernyőn.
