import sys
import os
from tkinter import Tk, Label, Button, Entry

PIPE_NAME = "/tmp/tk_to_c_pipe"

def send_confirmation_and_scores(confirmation, user_id, scores, root):
    """Send confirmation and scores to the C program via the pipe."""
    try:
        with open(PIPE_NAME, "w") as pipe:
            data = f"{confirmation}," + ",".join(map(str, scores))
            print(f"Sending to pipe: {data}")
            pipe.write(data)
    except FileNotFoundError:
        print("Error: Pipe not found.")
    except Exception as e:
        print(f"Error sending message: {e}")
    finally:
        root.destroy()

def display_scoring_window(goods, user_id, user_name):
    """Display scoring UI for goods."""
    root = Tk()
    root.title(f"Rate Goods for {user_name}")  # Window title includes user_name

    scores = []

    # Header Label
    Label(root, text=f"Rate the goods for User {user_name}:", font=("Arial", 14)).pack(pady=10)

    # Dynamic creation of labels and entry fields for scoring
    for idx, good in enumerate(goods):
        Label(root, text=f"{good}:").pack()
        score_entry = Entry(root, width=5)
        score_entry.pack(pady=5)
        score_entry.insert(0, "0")  # Default to 0
        scores.append(score_entry)

    # Confirm Scores Button
    Button(root, text="Confirm Scores", font=("Arial", 12), bg="green", fg="white",
           command=lambda: send_confirmation_and_scores(1, user_id,
                                                        [int(entry.get()) for entry in scores], root)).pack(pady=10)

    root.mainloop()

def main():
    if len(sys.argv) < 5:
        print("Usage: python tkinter_app.py <message> <user_id> <goods_list> <user_name>")
        return

    message = sys.argv[1]
    user_id = int(sys.argv[2])
    goods_list = sys.argv[3]  # Comma-separated goods list
    user_name = sys.argv[4]   # User name passed from C
    goods = goods_list.split(",")  # Split into a list of goods

    root = Tk()
    root.title(f"Confirmation for {user_name}")  # Window title includes user_name

    # Display message
    Label(root, text=message, font=("Arial", 14)).pack(pady=20)

    # Yes Button
    Button(root, text="Yes", font=("Arial", 12), bg="green", fg="white",
           command=lambda: [root.destroy(), display_scoring_window(goods, user_id, user_name)]).pack(side="left", padx=10)

    # No Button
    Button(root, text="No", font=("Arial", 12), bg="red", fg="white",
           command=lambda: send_confirmation_and_scores(0, user_id, [], root)).pack(side="left", padx=10)

    root.mainloop()

if __name__ == "__main__":
    main()




# import sys # this could work, but had some faults when got puted toghether with the c.
# import os
# from tkinter import Tk, Label, Button

# PIPE_NAME = "/tmp/tk_to_c_pipe"

# def send_confirmation(value, root):
#     """Send a numeric confirmation value to the C program (0 or 1) and close the Tk window."""
#     print("...Sending confirmation:", value)
#     try:
#         with open(PIPE_NAME, "w") as pipe:
#             pipe.write(str(value))
#             print("Data written to pipe successfully.")

#     except FileNotFoundError:
#         print("Error: Pipe not found.")
#     except Exception as e:
#         print(f"Error sending message: {e}")
#     finally:
#         root.destroy()  # Close the Tk window



# def create_tk_instance(message):
#     """Create a Tkinter window with yes/no buttons."""
#     root = Tk()
#     root.title("Confirmation Window")

#     # Display the message
#     Label(root, text=f"Message received: {message}", font=("Arial", 14)).pack(pady=20)

#     # Button frame
#     btn_frame = None
#     # Create a frame for the buttons if desired
#     # Commented out in case we just want them stacked, but it's nicer to group them
#     btn_frame = ButtonFrame(root, message, root)
#     btn_frame.pack(pady=10)

#     root.mainloop()

# class ButtonFrame:
#     def __init__(self, parent, message, root):
#         self.frame = parent
#         self.message = message
#         self.root = root
#         self.init_ui()

#     def init_ui(self):
#         # "Yes" button (send 1)
#         yes_button = Button(self.frame, text="Yes", font=("Arial", 12), width=10,
#                             command=lambda: send_confirmation(1, self.root))
#         yes_button.pack(side="left", padx=10)

#         # "No" button (send 0)
#         no_button = Button(self.frame, text="No", font=("Arial", 12), width=10,
#                            command=lambda: send_confirmation(0, self.root))
#         no_button.pack(side="left", padx=10)

#     def pack(self, **kwargs):
#         # This allows the ButtonFrame to be treated like a widget
#         # Just to keep code modular
#         f = Label(self.frame)  # Temporary label frame if needed
#         f.pack(**kwargs)

# def main():
#     if len(sys.argv) < 2:
#         print("Usage: python tkinter_app.py <message>")
#         return

#     message = sys.argv[1]
#     create_tk_instance(message)

# if __name__ == "__main__":
#     main()
