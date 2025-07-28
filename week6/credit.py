import sys


def main():
    # get user input
    card_str = input("Enter credit card: ")

    # convert string input to int
    card_num = int(card_str)

    # get str len
    digits = len(card_str)

    # process card to get type
    card_flag = get_card_type(card_num, digits)

    # check if card is valid (helper function)
    if not is_valid(card_num):
        card_flag = "INVALID"

    # print type
    print(card_flag)


def is_valid(card):
    x = 0

    # loop to sum
    while card > 0:
        last_digit = card % 10
        x += last_digit
        card = int(card / 10)
        before_last_digit = card % 10
        x += sum_of_double(before_last_digit)
        card = int(card / 10)

    # check if sum is valid
    if x % 10 != 0:
        return False
    else:
        return True


def sum_of_double(digit):
    # processes the before last digit
    doubled = int(digit * 2)
    if doubled >= 10:
        return (doubled % 10) + int(doubled / 10)
    else:
        return doubled


def get_card_type(card, digits):
    d = digits - 2
    power = pow(10, d)
    first_2_numbers = int(card / power)
    if first_2_numbers in (34, 37) and digits == 15:
        return "AMEX"
    elif first_2_numbers in range(51, 56) and digits == 16:
        return "MASTERCARD"
    elif int(first_2_numbers / 10) == 4 and digits in (13, 16):
        return "VISA"
    else:
        return "INVALID"


main()
