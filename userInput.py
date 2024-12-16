import tkinter as tk
from tkinter import messagebox
import os

current_dir = os.path.dirname(os.path.abspath(__file__))
file_path = os.path.join(current_dir, "shopping_data.txt")

class ShoppingApp:
    def __init__(self, master):
        self.master = master
        self.master.title("Buy by Threads")
        self.master.geometry("550x550")
        self.master.configure(bg="#e8e8e8")

        self.title_font = ("Helvetica", 18, "bold")
        self.label_font = ("Helvetica", 12)
        self.bold_font = ("Helvetica", 12, "bold")
        self.bg_color = "#f9f9f9"
        self.button_color = "#5cb85c"
        self.button_text_color = "white"

        self.item_rows = []

        self.create_title_label()
        self.create_user_frame()
        self.create_shopping_frame()
        self.create_action_frame()

        self.add_item_row()

    def create_title_label(self):
        # A frame at the top for the main title
        frame_title = tk.Frame(self.master, bg="#e8e8e8")
        frame_title.pack(pady=10)

        title_label = tk.Label(frame_title, text="Buy by Threads", font=self.title_font, bg="#e8e8e8", fg="#333333")
        title_label.pack()

    def create_user_frame(self):
        # User Information Frame
        self.frame_user = tk.LabelFrame(self.master, text="User Information", font=self.bold_font, bg=self.bg_color, padx=10, pady=10)
        self.frame_user.pack(padx=20, pady=10, fill="x")

        tk.Label(self.frame_user, text="User Name:", font=self.label_font, bg=self.bg_color).grid(row=0, column=0, padx=5, pady=5, sticky="w")
        self.entry_user = tk.Entry(self.frame_user, width=30, font=self.label_font)
        self.entry_user.grid(row=0, column=1, padx=5, pady=5)

        # Threshold Input (numbers only)
        vcmd_threshold = (self.master.register(self.validate_number), "%P")
        tk.Label(self.frame_user, text="Threshold:", font=self.label_font, bg=self.bg_color).grid(row=1, column=0, padx=5, pady=5, sticky="w")
        self.entry_threshold = tk.Entry(self.frame_user, width=30, font=self.label_font, validate="key", validatecommand=vcmd_threshold)
        self.entry_threshold.grid(row=1, column=1, padx=5, pady=5)

    def create_shopping_frame(self):
        # Shopping List Frame
        self.frame_list = tk.LabelFrame(self.master, text="Shopping List", font=self.bold_font, bg=self.bg_color, padx=10, pady=10)
        self.frame_list.pack(padx=20, pady=10, fill="both", expand=True)

        # A frame inside to hold item rows
        self.items_container = tk.Frame(self.frame_list, bg=self.bg_color)
        self.items_container.pack(fill="both", expand=True)

        # Add Row Button
        self.btn_add_row = tk.Button(self.frame_list, text="Add Item", font=self.bold_font, bg="#0275d8", fg="white", command=self.add_item_row)
        self.btn_add_row.pack(pady=10)

    def create_action_frame(self):
        # Action Frame (Save and Status)
        self.frame_action = tk.Frame(self.master, bg="#e8e8e8")
        self.frame_action.pack(pady=10)

        self.btn_submit = tk.Button(self.frame_action, text="Save", font=self.bold_font, bg=self.button_color, fg=self.button_text_color, command=self.save_data)
        self.btn_submit.pack(side="left", padx=10)

        self.label_status = tk.Label(self.frame_action, text="", font=self.label_font, bg="#e8e8e8")
        self.label_status.pack(side="left", padx=10)

    def validate_number(self, new_text):
        # This function checks if the new text is all digits or empty
        return new_text.isdigit() or new_text == ""

    def add_item_row(self):
        # Validation command for count entries (numbers only)
        vcmd = (self.master.register(self.validate_number), "%P")

        row_frame = tk.Frame(self.items_container, bg=self.bg_color)
        row_frame.pack(fill="x", pady=5)

        tk.Label(row_frame, text="Item:", font=self.label_font, bg=self.bg_color).pack(side="left", padx=5)
        item_entry = tk.Entry(row_frame, width=20, font=self.label_font)
        item_entry.pack(side="left", padx=5)

        tk.Label(row_frame, text="Count:", font=self.label_font, bg=self.bg_color).pack(side="left", padx=5)
        count_entry = tk.Entry(row_frame, width=10, font=self.label_font, validate="key", validatecommand=vcmd)
        count_entry.pack(side="left", padx=5)

        self.item_rows.append((item_entry, count_entry))

    def reset_item_rows(self):
        # Remove all item rows from the UI
        for (item_entry, count_entry) in self.item_rows:
            item_entry.master.destroy()
        self.item_rows = []

        # Add back a single empty row
        self.add_item_row()

    def save_data(self):
        username = self.entry_user.get().strip()
        threshold = self.entry_threshold.get().strip()

        if not username:
            messagebox.showwarning("Input Error", "Please provide a username.")
            self.label_status.config(text="Submission failed. Try again.", fg="red")
            return

        if not threshold:
            messagebox.showwarning("Input Error", "Please provide a threshold.")
            self.label_status.config(text="Submission failed. Try again.", fg="red")
            return

        # Validate item rows
        items = []
        for (item_entry, count_entry) in self.item_rows:
            item = item_entry.get().strip()
            count = count_entry.get().strip()

            # If both are empty, ignore this row
            if not item and not count:
                continue

            # If one is empty, it's invalid
            if not item or not count:
                messagebox.showwarning("Input Error", "Please fill out all item and count fields or remove empty rows before saving.")
                self.label_status.config(text="Submission failed. Try again.", fg="red")
                return

            items.append((item, count))

        if not items:
            messagebox.showwarning("Input Error", "Please provide at least one valid item and count.")
            self.label_status.config(text="Submission failed. Try again.", fg="red")
            return

        # Check if user already exists (to set is_repeated)
        is_repeated = 0
        if os.path.exists(file_path):
            with open(file_path, "r") as f:
                for line in f:
                    line = line.strip()
                    if not line:
                        continue
                    parts = line.split(",")
                    # parts[0] = username
                    if parts[0].strip() == username:
                        is_repeated = 1
                        break

        # Format data: username,is_repeated,threshold,good1,count1,good2,count2,...
        item_pairs = []
        for (item, count) in items:
            item_pairs.append(item)
            item_pairs.append(count)

        final_line = ",".join([username, str(is_repeated), threshold] + item_pairs)

        # Write to file
        with open(file_path, "a") as file:
            file.write(f"{final_line}\n")

        # Clear fields after submission
        self.entry_user.delete(0, tk.END)
        self.entry_threshold.delete(0, tk.END)
        self.reset_item_rows()

        self.label_status.config(text="Data saved successfully!", fg="green")


if __name__ == "__main__":
    root = tk.Tk()
    app = ShoppingApp(root)
    root.mainloop()
