import glfw

if not glfw.init():
    raise SystemExit("Failed to initialize GLFW")

glfw.window_hint(glfw.VISIBLE, glfw.TRUE)
window = glfw.create_window(640, 480, "Hello, Wayland!", None, None)
if not window:
    glfw.terminate()
    raise SystemExit("Failed to create GLFW window")

glfw.make_context_current(window)

while not glfw.window_should_close(window):
    glfw.poll_events()
    glfw.swap_buffers(window)

glfw.terminate()