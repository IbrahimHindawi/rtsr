import sdl2

block:
  var result = init(INIT_EVERYTHING)
  if result == SdlError.toBool:
    echo getError()
    quit(-1)
var window = createWindow("RTSR", 
            SDL_WINDOWPOS_UNDEFINED.int32,SDL_WINDOWPOS_UNDEFINED,
            800, 640, 0)
