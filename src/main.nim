import sdl2

var
  window: WindowPtr
  renderer: RendererPtr
  isRunning: bool

  windowWidth = 800
  windowHeight = 600
  colorBuffer: ptr uint32

proc initialize_window(): bool =
  var res = init(INIT_EVERYTHING)

  if res == SdlError.toBool:
    echo getError()
    result = false
    quit(-1)
  
  window = createWindow("RTSR", SDL_WINDOWPOS_CENTERED.int32, SDL_WINDOWPOS_CENTERED, 800, 640, SDL_WINDOW_BORDERLESS)  
  if (window == nil):
    echo "failed to create Window!"

  renderer = createRenderer(window, -1, 0)
  if renderer == nil:
    echo "failed to create Renderer!"

  result = true  

proc setup(): void =
  colorBuffer = cast[ptr uint32]( alloc( sizeof(uint32) * windowWidth * windowHeight) )
  if colorBuffer == nil:
    echo "Unable to initialize memory."

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
  setDrawColor(renderer, 32, 32, 32, 255)
  clear(renderer)
  present(renderer)

proc destroy(): void = 
  destroyRenderer(renderer)
  destroyWindow(window)

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