import sys
import os
from tkinter import Tk, Label, Button

PIPE_NAME = "/tmp/tk_to_c_pipe"

def send_confirmation(value, root):
    """Send a numeric confirmation value to the C program (0 or 1) and close the Tk window."""
    print("...Sending confirmation:", value)
    try:
        with open(PIPE_NAME, "w") as pipe:
            pipe.write(str(value))
            print("Data written to pipe successfully.")

    except FileNotFoundError:
        print("Error: Pipe not found.")
    except Exception as e:
        print(f"Error sending message: {e}")
    finally:
        root.destroy()  # Close the Tk window



def create_tk_instance(message):
    """Create a Tkinter window with yes/no buttons."""
    root = Tk()
    root.title("Confirmation Window")

    # Display the message
    Label(root, text=f"Message received: {message}", font=("Arial", 14)).pack(pady=20)

    # Button frame
    btn_frame = None
    # Create a frame for the buttons if desired
    # Commented out in case we just want them stacked, but it's nicer to group them
    btn_frame = ButtonFrame(root, message, root)
    btn_frame.pack(pady=10)

    root.mainloop()

class ButtonFrame:
    def __init__(self, parent, message, root):
        self.frame = parent
        self.message = message
        self.root = root
        self.init_ui()

    def init_ui(self):
        # "Yes" button (send 1)
        yes_button = Button(self.frame, text="Yes", font=("Arial", 12), width=10,
                            command=lambda: send_confirmation(1, self.root))
        yes_button.pack(side="left", padx=10)

        # "No" button (send 0)
        no_button = Button(self.frame, text="No", font=("Arial", 12), width=10,
                           command=lambda: send_confirmation(0, self.root))
        no_button.pack(side="left", padx=10)

    def pack(self, **kwargs):
        # This allows the ButtonFrame to be treated like a widget
        # Just to keep code modular
        f = Label(self.frame)  # Temporary label frame if needed
        f.pack(**kwargs)

def main():
    if len(sys.argv) < 2:
        print("Usage: python tkinter_app.py <message>")
        return

    message = sys.argv[1]
    create_tk_instance(message)

if __name__ == "__main__":
    main()



# import sys
# import os
# from tkinter import Tk, Label, Button

# PIPE_NAME = "/tmp/tk_to_c_pipe"

# def send_confirmation(message):
#     """Send a confirmation message to the C program via the named pipe."""
#     try:
#         with open(PIPE_NAME, "w") as pipe:
#             pipe.write(message)
#     except FileNotFoundError:
#         print("Error: Pipe not found.")
#     except Exception as e:
#         print(f"Error sending message: {e}")

# def create_tk_instance(message):
#     """Create a Tkinter window with a confirmation button."""
#     root = Tk()
#     root.title("Message Viewer")

#     # Display the message
#     Label(root, text=f"Message received: {message}", font=("Arial", 14)).pack(pady=20)

#     # Add a confirmation button
#     Button(root, text="Confirm", font=("Arial", 12), command=lambda: send_confirmation(
#         f"The user confirms: {message}")).pack(pady=10)

#     root.mainloop()

# def main():
#     if len(sys.argv) < 2:
#         print("Usage: python tkinter_app.py <message>")
#         return

#     message = sys.argv[1]
#     create_tk_instance(message)

# if __name__ == "__main__":
#     main()
