import ptrops
import sdl2

var
  window*: WindowPtr
  renderer*: RendererPtr
  isRunning*: bool

  windowWidth*: int = 800
  windowHeight*: int = 600
  colorBuffer*: ptr uint32
  colorBufferTexture*: TexturePtr

proc initialize_window*(): bool =
  var res = init(INIT_EVERYTHING)

  if res == SdlError.toBool:
    echo getError()
    result = false
    quit(-1)

  var displayMode: DisplayMode
  discard getCurrentDisplayMode(0.int32, displayMode)
  windowWidth = displayMode.w
  windowHeight = displayMode.h
  
  window = createWindow("RTSR", SDL_WINDOWPOS_CENTERED.int32, SDL_WINDOWPOS_CENTERED, windowWidth.int32, windowHeight.int32, SDL_WINDOW_BORDERLESS)  
  if (window == nil):
    echo "Error creating SDL Window!"

  renderer = createRenderer(window, -1, 0)
  if renderer == nil:
    echo "Error creating SDL Renderer!"

  discard setFullscreen(window, SDL_WINDOW_FULLSCREEN)

  result = true  

proc renderColorBuffer*(): void =
  updateTexture(colorBufferTexture, nil, colorBuffer, (windowWidth * sizeof(uint32)).int32)
  copy(renderer, colorBufferTexture, nil, nil)

proc clearColorBuffer*(color:uint32): void =
  for y in 0..<windowHeight:
    for x in 0..<windowWidth:
      colorBuffer[(windowWidth * y) + x] = color

proc drawGrid*(color:uint32, gridSize:int) =
  for y in 0..<windowHeight:
    for x in 0..<windowWidth:
      colorBuffer[windowWidth * y + x] = (x mod gridSize == 0 or y mod gridSize == 0).uint32 * color

proc drawRectangle*(color:uint32, posx, posy, w, h:int) =
  for y in 0..<windowHeight:
    for x in 0..<windowWidth:
      #colorBuffer[windowWidth * y + x] = (x mod gridSize == 0 or y mod gridSize == 0).uint32 * color
      if x > posx and x < posx+w and y > posy and y < posy+h:
        color_buffer[y * window_width + x] = color  