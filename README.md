Ez egy GFX könyvtár változat, amely képes kezelni az összes magyar ékezetes karaktert. Teszi ezt úgy, hogy az Adfruit drawChar függvényét lecseréli 16 bites karaktereket kezelni tudó változatra. A void visszatérés helyett az xa (xAdvanced) értékkel tér vissza. Igy képes a kernelésre is.
A magyar_font.h file-t a **Bodmer** féle fontconvert programmal hoztam létre a megfelelő truetype font file-ból
