import pygame
import serial
import time

# ---------------------- CONFIGURE THESE ----------------------
SERIAL_PORT = 'COM5'     # Replace with your actual port (e.g., '/dev/ttyUSB0' on Linux/Mac)
BAUD_RATE = 115200
# -------------------------------------------------------------

# Initialize serial connection
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Wait for Arduino to initialize
    print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} baud.")
except serial.SerialException:
    print(f"Could not open serial port {SERIAL_PORT}.")
    exit()

# Initialize pygame
pygame.init()
screen = pygame.display.set_mode((1000, 1000))
pygame.display.set_caption("Mouse Tracker")

clock = pygame.time.Clock()

running = True
prevMessage = ""
isClicked = 0
time.sleep(2)

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                isClicked = 1

        elif event.type == pygame.MOUSEBUTTONUP:
            if event.button == 1:
                isClicked = 0
                
    # Serial
    mouse_pos = pygame.mouse.get_pos()
    try:
        x = max(min(1000-mouse_pos[0], 1000), 0)
        y = max(min(mouse_pos[1], 1000), 0)  # invert Y

        message = f"{x} {y} {isClicked}\n"
        if message != prevMessage:
            ser.flushOutput()
            ser.write(message.encode())
            print("Sent:", message.strip())
            prevMessage = message
    except Exception as e:
        print("Serial error:", e)
        running = False

    # Draw screen
    screen.fill((30, 30, 30))
    pygame.draw.circle(screen, (0, 255, 0), pygame.mouse.get_pos(), 5)
    pygame.display.flip()
    clock.tick(60)

# Cleanup
ser.close()
pygame.quit()
