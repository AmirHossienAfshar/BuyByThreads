import tkinter as tk
from tkinter import messagebox
import os

# File path for storing data
current_dir = os.path.dirname(os.path.abspath(__file__))
file_path = os.path.join(current_dir, "shopping_data.txt")

# Save user data to the file
def save_data():
    username = entry_user.get().strip()
    shopping_list = text_shopping.get("1.0", tk.END).strip()

    if username and shopping_list:
        # Format the shopping list to be comma-separated
        items = shopping_list.splitlines()
        formatted_list = ",".join(item.strip() for item in items if item.strip())

        # Write to the file in the desired format
        with open(file_path, "a") as file:
            file.write(f"{username}:{formatted_list}\n")
        
        # Clear fields after submission
        entry_user.delete(0, tk.END)
        text_shopping.delete("1.0", tk.END)
        
        label_status.config(text="Data saved successfully!", fg="green")
    else:
        messagebox.showwarning("Input Error", "Please fill out all fields.")
        label_status.config(text="Submission failed. Try again.", fg="red")

# Create the main window
root = tk.Tk()
root.title("Shopping List Manager")
root.geometry("500x400")
root.configure(bg="#e8e8e8")

# User Information Group Box
frame_user = tk.LabelFrame(root, text="User Information", font=("Helvetica", 12, "bold"), bg="#f9f9f9", padx=10, pady=10)
frame_user.pack(padx=20, pady=10, fill="x")

tk.Label(frame_user, text="User Name:", font=("Helvetica", 12), bg="#f9f9f9").grid(row=0, column=0, padx=5, pady=5, sticky="w")
entry_user = tk.Entry(frame_user, width=30, font=("Helvetica", 12))
entry_user.grid(row=0, column=1, padx=5, pady=5)

# Shopping List Group Box
frame_list = tk.LabelFrame(root, text="Shopping List", font=("Helvetica", 12, "bold"), bg="#f9f9f9", padx=10, pady=10)
frame_list.pack(padx=20, pady=10, fill="x")

text_shopping = tk.Text(frame_list, width=50, height=8, font=("Helvetica", 12), wrap="word")
text_shopping.pack(padx=5, pady=5)

# Submit Button and Status Label
frame_action = tk.Frame(root, bg="#e8e8e8")
frame_action.pack(pady=10)

btn_submit = tk.Button(frame_action, text="Save", font=("Helvetica", 12, "bold"), bg="#5cb85c", fg="white", command=save_data)
btn_submit.pack(side="left", padx=10)

label_status = tk.Label(frame_action, text="", font=("Helvetica", 12), bg="#e8e8e8")
label_status.pack(side="left", padx=10)

# Run the app
root.mainloop()
