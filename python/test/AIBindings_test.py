from AIBindings import Game
import pathlib

library_path = pathlib.Path().absolute() / "../build/lib/Bindings/libBindings-d.so"

def test_load_ai_bindings():
    with Game(library_path, 0) as game:
        for x in range(100):
            game.SkipDays(10)
    
