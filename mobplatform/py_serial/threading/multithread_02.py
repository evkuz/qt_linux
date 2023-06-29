# Python program to illustrate the concept
# of threading
# importing the threading module
import threading
import time


def print_OneSec(num):
    # function to print cube of given num
    while num <= 10:
        time.sleep(1.0)
        num += 1
        print("One second interval : {}" .format(num))
    print("One Second Thread is finished")


def print_HalfSec(num):
    # function to print square of given num
    while num <= 10:
        time.sleep(0.5)
        num += 0.5
        print("Half second interval : {}".format(num))
    print("Half Second Thread is finished")


if __name__ == "__main__":
    # creating thread
    t1 = threading.Thread(target=print_OneSec, args=(1,))
    t2 = threading.Thread(target=print_HalfSec, args=(1.0,))

    # starting thread 1
    t1.start()
    # starting thread 2
    t2.start()

    # wait until thread 1 is completely executed
    t1.join()
    # wait until thread 2 is completely executed
    t2.join()

    # both threads completely executed
    print("Done!")
