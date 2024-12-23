import tkinter as tk

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
        while i < len(self.tasks) - 1:
            if self.tasks[i].time_of_completion == self.time:
                self.removeTask(i+1)
            else:
                i+=1

if __name__ == "__main__":
    ram = RAM(10)
    ram.showMemory()
    ram.showTasks()
    ram.addTask('task1', 5, 3)
    ram.addTask('task2', 3, 7)
    ram.showMemory()
    ram.showTasks()
    for _ in range(10):
        ram.incrementTime()
        print(f"TIME:{ram.time}")
        ram.showMemory()
        ram.showTasks()

