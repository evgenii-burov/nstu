from tkinter import Tk, Label, Button, Entry,  StringVar, font, ttk
import time
class Task:
    def __init__(self, name, size, time):
        self.name = name
        self.size = size
        self.time = time
        self.time_left = time
        self.start_time = 0

class RAM:
    def __init__(self, size):
        self.size = size
        self.array = [0] * size
        self.tasks = []
        self.time = 0

    def addTask(self, name, size, time):
        if self.checkIfEnoughMem(size):
          for i in range(self.size):
              if self.array[i] == 0 and size > 0:
                  self.array[i] = 1
                  size -= 1
          newTask = Task(name, size, time)
          newTask.start_time = self.time
          self.tasks.append(newTask)
    def checkIfEnoughMem(self, size):
        available_space = 0
        for i in range(self.size):
            if self.array[i] == 0:
                available_space += 1
        return available_space >= size
    def updateTime(self):
        self.time +=1
        for task in self.tasks:
            task.time_left -= 1
            if task.time_left == 0:
                self.releaseMemory(task)
    def releaseMemory(self, task):
        #Remove the task
        self.tasks.remove(task)
        for i in range(self.size):
            if self.array[i] == 1:
              self.array[i] = 0

def update():
      global ram
      ram.updateTime()
      memory_str = ''
      for i in range(len(ram.array)):
        memory_str += str(ram.array[i]) + ' '
      memory.config(text = memory_str)
      tasks_str = ''
      for task in ram.tasks:
        tasks_str += f"Task: {task.name}, size: {task.size}, time_left: {task.time_left}, start_time: {task.start_time}\n"
      tasks_text.set(tasks_str)
def setMemory():
      global ram
      size = int(memory_entry.get())
      ram.addTask('test',size,10)
      update()


if __name__ == "__main__":
    global ram
    ram = RAM(1024)

    window = Tk()
    window.title('Эмулятор распределителя оперативной памяти')
    window.geometry('1600x900')


    # --- Configure Grid Columns ---
    for i in range(10):  # Adjust the number of columns as needed
        window.columnconfigure(i, weight=1)

    # Headings
    memory_headline = Label(window, text='Memory array:', font=('Arial', 8), fg='blue', justify="left")
    memory_headline.grid(row=1, column=5, sticky="w", padx=10, pady=10) # Left align with padding

    tasks_headline = Label(window, text='Current tasks:', font=('Arial', 20), fg='blue', justify="left")
    tasks_headline.grid(row=1, column=7, sticky="w", padx=10, pady=10)

    # Displays
    memory = Label(window, text='', font=('Arial', 20), justify="left")
    memory.grid(row=2, column=5, sticky="w", padx=10, pady=5)

    tasks_text = StringVar()  # To update the text of the tasks label
    tasks = Label(window, textvariable=tasks_text, font=('Arial', 10), justify="left")  # Added justification
    tasks.grid(row=2, column=7, sticky="w", padx=10, pady=5)

    #Time increment
    time_increment = Button(window, text='increment time', command=update)
    time_increment.grid(row=6, column=1, padx=10, pady=10, sticky="w")
    #Memory set button
    memory_entry = Entry(window)
    memory_entry.grid(row=8,column=1, padx=10, pady=5, sticky="ew") # Expand horizontally within column

    set_memory = Button(window, text='set memory', command=setMemory)
    set_memory.grid(row=10, column=1, padx=10, pady=10, sticky="w") # Align to the left

    #Task add widgets
    task_name_entry = Entry(window)
    task_name_entry.grid(row=16, column=1, padx=10, pady=5, sticky="ew")

    task_size_entry = Entry(window)
    task_size_entry.grid(row=16, column=2, padx=10, pady=5, sticky="ew")

    task_time_entry = Entry(window)
    task_time_entry.grid(row=16, column=3, padx=10, pady=5, sticky="ew")


    update()
    ram.addTask('task1', 123, 5)


    window.mainloop()
