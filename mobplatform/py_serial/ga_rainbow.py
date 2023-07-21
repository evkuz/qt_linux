# Пример решения задачи с помощью ГА.
# Имеем исходную строку "каждый охотник желает знать где сидит фазан".
# 1. Генерим строку со случайным набором символов, но равной по длине исходной строке. Это начальная популяция.
# 2. Меняем случайным образом в строке-популяции по 1 символу, отбираем наиболее подходящий вариант (потомок).
#    Наиболее подходящий - это максимальное число совпадений у потомка с целевой строкой.
# 3. Потомок становится предком, идем в п.2. повторяем, пока не получем точное совпадение.
#
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


# Смешиваем символы внутри строки случайным образом
# Наиболее удачным оказался shuffle.
def mixChars(theText):
    stLength = len(theText)
    #result = " ".join(theText.split())
    #result.replace(" ", "")
    #result.strip()
    result = theText.translate({ord(c): None for c in string.whitespace})
    #l = list(result)
    #random.shuffle(l)

    return result


# Вставить пробел в случайную позицию строки
# На деле это не нужно, т.к. пробел включен в список(набор) символов из которого делается выборка.
def insert_space(theText):
    r = random.randint(1, len(theText)-1)
    return theText[:r] + ' ' + theText[r:]


#   Ф-ция пригодности.
#   Это сердце ГА. Принимает на вход потомка и целевую строку, на выходе дает число совпадений,
#   т.е ОБЩЕЕ число совпавших символах в этих строках, даже если символы идут не подряд.
def fitnessFunc(theText, theSource):
    matchedString = ""
    nOfMatching = 0     # Количество совпадений
    maxMatched = 0
    for element in range(0, len(theText)):
        if theText[element] == theSource[element]:
            matchedString += theText[element]   # ДОбавляем совпавшй символ к имеющейся строке
            nOfMatching += 1
            if nOfMatching > maxMatched:
                maxMatched = nOfMatching
        else:   # Когда строки совпадают кусками из разных частей. При этом сохраняется maxMatched
            matchedString = ""
    # Возвращаем словарь, где ключ - число максимальных совпадений, а значение - сама строка
    return maxMatched


#   Генерим случайную строку длиной strLen
def genRandomString(strLen):
    global sample_string
    res = ''.join((random.choice(sample_string)) for x in range(strLen))
    return res


#   Операция мутации в терминах ГА.
#   Меняем 1 раз случайный символ строки на случайный символ алфавита кириллицы, - это строка sample_string
def strModification(theText):
    global  sample_string
    strLen = len(theText)
    sampleLen = len(sample_string)
    randIndex = random.randint(0, strLen-1)
    randChar = sample_string[random.randint(0, sampleLen-1)]
    modifiedStr = theText[:randIndex] + randChar + theText[randIndex + 1:]
    return modifiedStr


# Генерация потомков с последующим отбором.
# Отбираем тех, у кого число совпадений превышает таковое у предка.
# 10 раз вызываем модификацию строки (мутация), анализируем результат на совпадение с исходной строкой - Селекция.
# Меняем только 1 символ за модификацию
# Если хотя бы 1 символ совпадает, добавляем потомка в список, где каждый элемент это словарь
# {"число совпадений": "строка-потомок"}
# Ф-ция возвращает словарь {"число совпадений": "строка-потомок"}
# Параметр theText - строка-предок
# Параметр numOfMatched - число совпадений в строке-предок
def genOffSprings(theText, numOfMatched):
    maxMatched = 0
    myDict = {}
    for i in range(10):
        #   mixedStr = string_utils.shuffle(randomString)
        mixedStr = strModification(theText)
        numMatch = fitnessFunc(mixedStr, originalString)
        if numMatch > numOfMatched:
            myDict.update({numMatch: mixedStr})
            # Вот тут уже можно выходить из ф-ции, даже если i < 10
            # Вот тут добавляем строку в словарь где ключ- число совпадений, а значение - строка !
    return myDict    # maxMatched


if __name__ == "__main__":
    sample_string = 'абвгдеёжзийклмнопрстуфхцчшщъыьэюя '
    originalString = "каждый охотник желает знать где сидит фазан"
    print(originalString, " ", len(originalString))
    # randomString = "щьтбшхшжкёшъъчсы глфцлопчруыихзяшщручяастбс"
    # Начальная популяция.
    randomString = str(genRandomString(len(originalString)))
    #   print(len(originalString))
    #   Получили начачльную популяцию, проверяем её "сходимость" с исходной строкой
    print(randomString)
    initialMatch = fitnessFunc(randomString, originalString)
    print("Начальная популяция, число совпадений ", str(initialMatch))
    matching = []   # Список, где каждый элемент это словарь {"число совпадений" : "Строка"}
    result = {}     # словарь совпадений
    numOfMutation = 0   # Число мутаций, понадобившееся для полного совпадения строк. Для наглядности.
    totalMatch = initialMatch
    resultString = randomString
    while totalMatch < len(originalString):
        result = genOffSprings(resultString, totalMatch)  # Получили словарь совпадений, состоящий из 1 пары ключ-значение
        # Вот тут анализируем поколения, выбираем лучшее - это 1 строка.
        # Если несколько одинаковых по совпадениям строк - берм последнюю, более "молодую"
        theList = result.keys()
        if theList: # Если список не пустой, значит есть потомки, более подходящие, чем предок
            totalMatch += 1
            keysList = result.keys()
            theKey = max(keysList)
            resultString = result.get(theKey)
            matching.append(result)
        numOfMutation += 1
    # Пишем в файл полученный список
    with open("matchList.txt", "wt") as data:
        for item in matching:
            data.write("%s\n" % item)

    # example_string = "test"
    # print(example_string.replace(example_string[0], "b",1))
    print("...")
    print("Число мутаций ", numOfMutation)
