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

---


### Shopping List Manager - User Input Guide

here is the explanation of how user input is processed and stored in a text file, using Tkinter-based python file.

The Shopping List Manager collects a username and a list of shopping items through a graphical user interface (GUI) built with Tkinter. after submission, the data is stored in a text file shopping_data.txt in a structured format.

#### How User Input Works

Username Field: A single-line entry for the user's name.
Shopping List Field: A multi-line text box for entering shopping items, with one item per line.

#### Data Submission Process

When the user clicks the "Save" button:

- Data Collection:
    The username is collected from the entry field.
    The shopping list is collected from the dynammicaly-handled text boxes (there is a btn to add item, which allocates a new input), and the same for count.

The username and the shopping list are joined with a colon `:`

the output format is this:
```pthon
username:good1,count1,good2,count2,...
```

subdirectories of each stor:
Apparel, Beauty, Digital, Food, Home, Market, Sports, Toys