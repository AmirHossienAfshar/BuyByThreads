# BuyByThreads
### project schema

#### Main Menu

- A Tkinter form with a text field for entering the username and a button to submit.
- Clicking the button:
    1. Passes the username to the C program to initialize a new process.
    2. Opens a new page in Tkinter for goods selection.

#### Goods Selection Page

- Tkinter GUI with:
    1. Text fields or dropdowns for entering/selecting goods.
    2. A "Submit" button to send the goods list to the C program.

#### C Backend Process

- On submission:
    1. Tkinter writes the username and goods list to a file (or passes them as command-line arguments).
    2. The C program:
        Processes the input.
        Computes the goods' price and other parameters.
        Writes the results to an output file (or returns them via stdout).

#### Result Display

- Tkinter reads the results (file or output) and updates the GUI with the prices and parameters.
---

### Key Design Principles

#### Frontend (Tkinter):
- Use one main menu page to register or select users.
- Each user can open a new "session" (a new Tkinter page).
- Each session can interact with the backend independently.

#### Backend (C):
- Spawn a new process in the C backend for each user session.
- Keep user data isolated by associating unique input/output files or process identifiers for each user.