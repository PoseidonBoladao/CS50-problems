END_SENTENCE = [".", "?", "!"]
CONSTANTS = [100, 0.0588, 0.296, 15.8]

text = input("Text : ")
str_len = len(text)

spaces = 0
sentences = 0
letter = 0

for _ in range(str_len):
    if text[_].isalpha():
        letter += 1

    elif text[_] == " ":
        spaces += 1

    elif text[_] in END_SENTENCE:
        sentences += 1

words = spaces + 1
l = (letter / words * CONSTANTS[0])
s = (sentences / words * CONSTANTS[0])
index = (CONSTANTS[1] * l) - (CONSTANTS[2] * s) - CONSTANTS[3]
rounded = round(index)

if (rounded < 1):
    print("Before Grade 1")

elif (rounded > 16):
    print("Grade 16+")

else:
    print("Grade " + str(rounded))
