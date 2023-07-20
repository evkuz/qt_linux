# Строку "каждый охотник желает знать где сидит фазан" преобразуем в набор символов, составляюищх эту строку.
# there is no sadder story in the world than the story of romeo and juliet
# Добавляем пробел в случайную позицию
# Далее, через ГА восстанавливаем строку.
# 1. Генерация случайного поколения особей-решений
# 2. Выбор наиболее качественных особей поколения
# 3. Скрещивание особей для получения их потомков
# 4. Мутация потомков
# 5. Получение нового поколения


import random
import string_utils
import string


def mixChars(theText):
    stLength = len(theText)
    #result = " ".join(theText.split())
    #result.replace(" ", "")
    #result.strip()
    result = theText.translate({ord(c): None for c in string.whitespace})
    #l = list(result)
    #random.shuffle(l)

    return result


def insert_space(theText):
    r = random.randint(1, len(theText)-1)
    return theText[:r] + ' ' + theText[r:]


def fitnessFunc(theText, theSource):
    matchedString = ""
    nOfMatching = 0     # Количество совпавших символов подряд
    maxMatched = 0
    totalMatching = 0       # Всего совпадений в этой строке, т.е. сколько всего символов совпадает

    for element in range(0, len(theText)):
        if theText[element] == theSource[element]:
            matchedString += theText[element]   # ДОбавляем совпавшй символ к имеющейся строке
            nOfMatching += 1
            totalMatching += 1
            if nOfMatching > maxMatched:
                maxMatched = nOfMatching
        else:
            matchedString = ""
    result = maxMatched
    # Возвращаем словарь, где ключ - число максимальных совпаденй, а значение - сама строка

    return result


def genRandomString(strLen):
    sample_string = 'абвгдеёжзиклмнопрстуфхцчшщъыьэюя '
    res = ''.join((random.choice(sample_string)) for x in range(strLen)) #      + string.whitespace

    # print result
    print(str(res)) #   "The generated random string : " +


def strModification(theText):
    # Меняем символ в строке со случайным индексом на другой случайный символ
    sample_string = 'абвгдеёжзиклмнопрстуфхцчшщъыьэюя '
    strLen = len(theText)
    sampleLen = len(sample_string)
    randIndex = random.randint(0, strLen-1)
    randChar = sample_string[random.randint(0, sampleLen-1)]
    result = theText
    #result.replace(result[randIndex], randChar, 1)
    result = result[:randIndex] + randChar + result[randIndex + 1:]
    print("randIndex", " ", randIndex, ", ", "randChar ", randChar)
    print(result)
    return result


def genOffSprings(theText):
    maxMatched = 0
    myDict = {}
    for i in range(10):
        #   mixedStr = string_utils.shuffle(randomString)
        mixedStr = strModification(randomString)
        numOfMatched = fitnessFunc(mixedStr, myText)
        if numOfMatched > 0:
            myDict.update({numOfMatched: mixedStr})
        if numOfMatched > maxMatched:
            maxMatched = numOfMatched
            # Вот тут добавляем строку в словарь где ключ- число совпадений, а значение - строка !
            # Потом из этого словаря всех заберем.
        print(mixedStr, numOfMatched)

    return myDict    # maxMatched


if __name__ == "__main__":
    myText = "каждый охотник желает знать где сидит фазан"
    print(myText, " ", len(myText))
    #   genRandomString(len(myText))
    #   print(len(myText))
    randomString = "щьтбшхшжкёшъъчсы глфцлопчруыихзяшщручяастбс"
    print(randomString)
    # myLength = mixChars(randomString)
    # print(myLength)
    # maxMatched = 0
    #
    # for i in range(10):
    #     #   mixedStr = string_utils.shuffle(randomString)
    #     mixedStr = strModification(randomString)
    #     numOfMatched = fitnessFunc(mixedStr, myText)
    #     if numOfMatched > maxMatched:
    #         maxMatched = numOfMatched
    #         # Вот тут добавляем строку в словарь где ключ- число совпадений, а значение - строка !
    #         # Потом из этого словаря всех заберем.
    #     print(mixedStr, numOfMatched)
    # #     spacedText = insert_space(mixedStr)
    numMatched = 0
    #matching = {}
    n = 0
    while numMatched < 1:
        #   numMatched = genOffSprings(randomString)
        matching = genOffSprings(randomString)
        theList = matching.keys()
        if theList:
            numMatched += 1

    print(numMatched)
    print(matching)

    # Вот теперь это надо зациклить :)

    #   Всего 1 совпадение из всех - это мало.
    #   Меняем randomString
    example_string = "test"
    print(example_string.replace(example_string[0], "b",1))

