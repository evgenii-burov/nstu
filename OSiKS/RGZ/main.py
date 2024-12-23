from tkinter import *
import time

from PIL.ImageColor import colormap
from numpy.ma.core import left_shift


class CannotAddTask(Exception):
    pass

class CannotRemoveTask(Exception):
    pass

class Task:
    def __init__(self, name:str, size:int, time_of_completion:int, first_block:int):
        self.name = name
        self.size = size
        self.time_of_completion = time_of_completion
        self.first_block = first_block

class RAM:
    def __init__(self, size):
        self.memory_size = size
        self.memory_array = [0]*self.memory_size
        self.tasks = []
        self.time=0

    def showMemory(self):
        print(self.memory_array)

    def showTasks(self):
        i=1
        print("#\tname\tsize\ttime_of_completion")
        for task in self.tasks:
            print(f"{i}\t{task.name}\t{task.size}\t{task.time_of_completion}")
            i+=1

    def addTask(self, name, size, time_to_complete):
        sequential_free_memory = 0
        free_memory_start = 0
        for i in range(self.memory_size):
            if self.memory_array[i] == 0:
                sequential_free_memory +=1
            if sequential_free_memory == size:
                self.tasks.append(Task(name,size,time_to_complete+self.time,free_memory_start))
                for j in range(free_memory_start, free_memory_start+size):
                    self.memory_array[j]=1
                return
            if self.memory_array[i] == 1:
                if (i+1) > (self.memory_size-1):
                    raise CannotAddTask
                sequential_free_memory = 0
                free_memory_start = i+1
        raise CannotAddTask

    def removeTask(self, task_number):
        if task_number < 1:
            raise ValueError
        if task_number > len(self.tasks):
            raise CannotRemoveTask
        for i in range(self.tasks[task_number-1].first_block, self.tasks[task_number-1].first_block+self.tasks[task_number-1].size):
            self.memory_array[i]=0
        self.tasks.pop(task_number-1)

    def incrementTime(self):
        self.time+=1
        i=0
        while i < len(self.tasks):
            if self.tasks[i].time_of_completion == self.time:
                print(f"REMOVED")
                self.removeTask(i+1)
            else:
                i+=1

    def defragmentMemory(self):
        self.memory_array = [0]*self.memory_size
        sum_of_sizes = 0
        for task in self.tasks:
            sum_of_sizes += task.size
        if sum_of_sizes > self.memory_size:
            raise Exception
        for i in range(sum_of_sizes):
            self.memory_array[i]=1

def update():
    #memory array render
    m=''
    for i in range(ram.memory_size):
        if ram.memory_array[i] == 0:
            m+='0'
        else:
            m+='#'
        if i%50==49:
            m+='\n'
    print(m)
    memory.config(text=m)

    #tasks render
    t=''
    i = 1
    t+="№\tName\tSize\tTime of completion\n"
    for task in ram.tasks:
        t+=f"{i}\t{task.name}\t{task.size}\t{task.time_of_completion}\n"
        i += 1
    tasks.config(text=t, font=8)

def setMemory():
    global ram
    ram=RAM(int(memory_entry.get()))
    update()

if __name__ == "__main__":
    global ram
    ram = RAM(1024)
    window = Tk()
    window.title('Эмулятор распределителя оперативной памяти')
    window.geometry('1600x900')
    #Headlines
    memory_headline = Label(window, text='Memory array:', font=('Arial', 20), fg='blue', justify="left")
    memory_headline.grid(row=1, column=5)

    tasks_headline = Label(window, text='Current tasks:', font=('Arial', 20),fg='blue', justify="left")
    tasks_headline.grid(row=1, column=7)
    #Displays
    memory = Label(window, text='', font=('Arial',8), justify="left")
    memory.grid(row=2, column=5)

    tasks = Label(window, text='', font=('Arial',12), anchor='n')
    tasks.grid(row=2, column=7, sticky='n')
    #Time increment
    time_increment = Button(window, text='increment time', command=update)
    time_increment.grid(row=6, column=1)
    #Memory set button
    memory_entry = Entry(window)
    memory_entry.grid(row=8,column=1)

    set_memory = Button(window, text='set memory', command=setMemory)
    set_memory.grid(row=10, column=1)
    #Task add button
    task_name_entry = Entry(window)
    task_name_entry.grid(row=16, column=1)

    task_size_entry = Entry(window)
    task_size_entry.grid(row=16, column=2)

    task_time_entry = Entry(window)
    task_time_entry.grid(row=16, column=3)

    update()
    ram.addTask('task1', 123, 5)

    window.mainloop()
