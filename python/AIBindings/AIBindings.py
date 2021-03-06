import ctypes
import pathlib
from utils import memorize 

@memorize
def load_ai_shared_lib(libray_path):
    game_library  = ctypes.CDLL(libray_path)
    game_library.gameCreate.restype = ctypes.c_void_p
    return game_library

class Game:
    def __init__(self, library_path, seed):
        self.game_library = load_ai_shared_lib(library_path)
        self.game = self.game_library.gameCreate(ctypes.c_int(seed))

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.game_library.gameDestroy(self.game)

    def __getattr__(self, name):
        return lambda *x : self.game_library.__getattr__('game' + name)(self.game, *x)

