import sys

def checktypeClus(str):
    try:
        bol = (int(str) == float(str))
        return not bol
    except:
        print("Invalid number of clusters!")
        sys.exit(1)


def checktypeIter(str):
    try:
        bol = (int(str) == float(str))
        return not bol
    except:
        print("Invalid maximum iteration!")
        sys.exit(1)



def ECDis(arr1, arr2):
    sum = 0
    for i in range(len(arr1)):
        sum += ((arr1[i] - arr2[i]) ** 2)
    sum = sum ** 0.5
    return sum


def updCentroid(group, centroids, place):
    sum =0
    res=[]
    if len(group) == 0:
        centroids[place] = [0.0]*d
    else:
        for i in range(d):
            for j in range(len(group)):
                sum += group[j][i]
            sum = sum / len(group)
            res.append(sum)
            sum = 0
        centroids[place] = res


def printAll(centroids):
    for cent in centroids:
        line = ""
        for num in cent:
            num = '%.4f' % num
            line = line + "," + str(num)
        print(line[1::])

def empty(groups):
    for i in range(K):
        groups[i] = []

arglist = sys.argv
if len(sys.argv) == 4:
    try:
        K = int(arglist[1])
    except ValueError:
        print("Invalid number of clusters!")
        quit()
    try:
        iteration_number = int(arglist[2])
    except ValueError:
        print("Invalid maximum iteration!")
        quit()

    if (checktypeClus(arglist[1]) or int(arglist[1]) < 2):
        print("Invalid number of clusters!")
        sys.exit(1)
    if (checktypeIter(arglist[2]) or int(arglist[2]) < 1 or int(arglist[2]) > 1000):
        print("Invalid maximum iteration!")
        sys.exit(1)
    In_filename = arglist[3]

elif len(sys.argv) == 3:
    try:
        K = int(arglist[1])
    except ValueError:
        print("Invalid number of clusters!")
        quit()

    if (checktypeClus(arglist[1]) or int(arglist[1]) < 2):
        print("Invalid number of clusters!")
        sys.exit(1)

    In_filename = arglist[2]

if 'iteration_number' not in locals():
    iteration_number = 200

try:
    file = open(In_filename, 'r')
except:
    print("An Error Has Occurred")
    quit()

if (file == None):
    print("An Error Has Occurred")
    quit()

try:
    lines = file.read().split('\n')[:-1]
except:
    print("An Error Has Occurred")
    quit()
if len(lines) < K:
    print("Invalid number of clusters!")
    quit()

try:
    file.close()
except:
    print("An Error Has Occurred")
    quit()

vectors = []
centroids = []
groups = []
done = [False]*K
cnt = 0
for line in lines:
    arr1 = line.split(",")
    arr = [float(x) for x in arr1]
    vectors.append(arr)
    if cnt < K:
        centroids.append(arr)
        groups.append([])
        cnt+=1
d = len(arr)
cnt =0

while(done != [True]*K or cnt == iteration_number - 1):

    empty(groups)
    for i in range(len(vectors)):
        min = sys.maxsize
        indexJ = 0
        for j in range(len(centroids)):
            cur = ECDis(centroids[j], vectors[i])
            if (cur <= min):
                min = cur
                indexJ = j
        groups[indexJ].append(vectors[i])

    for i in range(K):
        copy = centroids[i].copy()
        updCentroid(groups[i], centroids, i)
        if ECDis(copy, centroids[i]) < 0.001:
            done[i] = True
        else:
            done[i] = False
    cnt += 1

printAll(centroids)
sys.exit(1)




