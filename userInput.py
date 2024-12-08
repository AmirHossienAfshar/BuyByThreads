import tkinter as tk
import os
# Determine the path of the current Python script
current_dir = os.path.dirname(os.path.abspath(__file__))
file_path = os.path.join(current_dir, "shopping_data.txt")

# Save user data to the file
def save_data():
    username = entry_user.get()
    shopping_list = text_shopping.get("1.0", tk.END).strip()

    if username and shopping_list:
        with open(file_path, "a") as file:
            file.write(f"{username}:{shopping_list}\n")
        label_status.config(text="Data saved successfully!", fg="green")
    else:
        label_status.config(text="Please fill out all fields.", fg="red")
# Create the GUI
root = tk.Tk()
root.title("Shopping List App")

tk.Label(root, text="User Name:").pack(pady=5)
entry_user = tk.Entry(root, width=30)
entry_user.pack(pady=5)

tk.Label(root, text="Shopping List:").pack(pady=5)
text_shopping = tk.Text(root, width=40, height=10)
text_shopping.pack(pady=5)

tk.Button(root, text="Save", command=save_data).pack(pady=10)
label_status = tk.Label(root, text="")
label_status.pack(pady=5)

root.mainloop()
