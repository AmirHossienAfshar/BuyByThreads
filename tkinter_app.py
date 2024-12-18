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
    root.title(f"Rate Goods for {user_name}")

    scores = []


    Label(root, text=f"Rate the goods for User {user_name}:", font=("Arial", 14)).pack(pady=10)


    for idx, good in enumerate(goods):
        Label(root, text=f"{good}:").pack()
        score_entry = Entry(root, width=5)
        score_entry.pack(pady=5)
        score_entry.insert(0, "0")  # Default to 0
        scores.append(score_entry)


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
    goods_list = sys.argv[3]
    user_name = sys.argv[4]
    goods = goods_list.split(",")

    root = Tk()
    root.title(f"Confirmation for {user_name}")

    Label(root, text=message, font=("Arial", 14)).pack(pady=20)

    Button(root, text="Yes", font=("Arial", 12), bg="green", fg="white",
           command=lambda: [root.destroy(), display_scoring_window(goods, user_id, user_name)]).pack(side="left", padx=10)

    Button(root, text="No", font=("Arial", 12), bg="red", fg="white",
           command=lambda: send_confirmation_and_scores(0, user_id, [], root)).pack(side="left", padx=10)

    root.mainloop()

if __name__ == "__main__":
    main()



