import sys
import os
from tkinter import Tk, Label, Button

PIPE_NAME = "/tmp/tk_to_c_pipe"

def send_confirmation(message):
    """Send a confirmation message to the C program via the named pipe."""
    try:
        with open(PIPE_NAME, "w") as pipe:
            pipe.write(message)
    except FileNotFoundError:
        print("Error: Pipe not found.")
    except Exception as e:
        print(f"Error sending message: {e}")

def create_tk_instance(message):
    """Create a Tkinter window with a confirmation button."""
    root = Tk()
    root.title("Message Viewer")

    # Display the message
    Label(root, text=f"Message received: {message}", font=("Arial", 14)).pack(pady=20)

    # Add a confirmation button
    Button(root, text="Confirm", font=("Arial", 12), command=lambda: send_confirmation(
        f"The user confirms: {message}")).pack(pady=10)

    root.mainloop()

def main():
    if len(sys.argv) < 2:
        print("Usage: python tkinter_app.py <message>")
        return

    message = sys.argv[1]
    create_tk_instance(message)

if __name__ == "__main__":
    main()
