import tkinter as tk
import subprocess

def send_message():
    # Send the "button_clicked" message to the C program
    process.stdin.write("button_clicked\n")
    process.stdin.flush()  # Ensure the message is sent
    print("Message sent to C program!")

# Start the C program as a subprocess
process = subprocess.Popen(
    r"C:\Users\user\Desktop\tk samples\pipe/server",  # Replace with your .exe file on Windows
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE,
    text=True  # Use text mode for strings
)

# Tkinter GUI
root = tk.Tk()
root.title("Python-C IPC")

btn = tk.Button(root, text="Click Me", command=send_message)
btn.pack(pady=20)

root.mainloop()

# Close the subprocess when the Tkinter app is closed
process.terminate()
process.wait()

