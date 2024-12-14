import sys
from tkinter import Tk, Label
from multiprocessing import Process

def create_tk_instance(message):
    """Create a Tkinter window displaying the message."""
    root = Tk()
    root.title("Message Viewer")
    Label(root, text=f"Message received: {message}", font=("Arial", 14)).pack(pady=20)
    root.mainloop()

def main():
    if len(sys.argv) < 2:
        print("Usage: python tkinter_app.py <message>")
        return

    message = sys.argv[1]
    process = Process(target=create_tk_instance, args=(message,))
    process.start()
    process.join()

if __name__ == "__main__":
    main()
