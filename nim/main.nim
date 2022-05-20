import sdl2
import ptrops
import display

proc setup(): void =
  colorBuffer = cast[ptr uint32]( alloc( sizeof(uint32) * windowWidth * windowHeight) )
  if colorBuffer == nil:
    echo "Unable to initialize memory."
  colorBufferTexture = createTexture(renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, windowWidth.int32, windowHeight.int32)

proc input(): void =
  var event: Event
  discard pollEvent(event)
  case event.kind:
    of QuitEvent:
      isRunning = false
    of KeyDown, KeyUp:
      if event.key.keysym.sym == K_ESCAPE:
        isRunning = false;
    else:
      discard

proc update(): void =
  discard

proc render(): void =
  setDrawColor(renderer, 0, 0, 0, 255)
  clear(renderer)

  renderColorBuffer()
  clearColorBuffer(0x00FFFFFF'u32)
  drawGrid(0x00FFFFFF'u32, 40)
  drawRectangle(0xFF0000FF'u32, 100, 100, 60, 160)

  present(renderer)

proc destroy(): void = 
  destroyRenderer(renderer)
  destroyWindow(display.window)

proc main(): void = 
  isRunning = initialize_window() 
  setup()
  while (isRunning == true):
    input()
    update()
    render()
  dealloc(colorBuffer)
  destroy()

main()