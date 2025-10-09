import sys
from rich import print
from time import sleep
import cursor

def printLyrics():

    cursor.hide()

    try:
        lyrics = [
            ("", 0.01),
            ("Baby I'm here", 0.06),
            ("I'm here to stay", 0.07),
            ("I ain't goin nowhere", 0.07),
            ("I know you're scared", 0.06),
            ("cause you been hurt", 0.07),
            ("baby it's alright", 0.08),
            ("lost in your eyes", 0.07),
            ("everytime that you look in mine", 0.09), #8
            ("promise to be all that you need", 0.09),
            ("i won't leave you baby", 0.08),
            ("no i just wanna love you", 0.07),
            ("I will never ever put nobody up above you", 0.055),
            ("I just wanna kiss you baby", 0.035),
            ("I just wanna hug you 'til the end", 0.035),
            ("baby 'til the end", 0.04)
        ]

        #add 13 to the front
        delays = [13, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4,
                0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4]

        for i, (line, char_delay) in enumerate(lyrics):
            for char in line:
                print(char, end='', flush = True)
                sleep(char_delay)
            sleep(delays[i])
            print('')

    finally:
        cursor.show()
    
printLyrics()