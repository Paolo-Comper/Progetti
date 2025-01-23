Input = [[0,0,0],
         [0,0,1],
         [0,1,0],
         [0,1,1],
         [1,0,0],
         [1,0,1],
         [1,1,0],
         [1,1,1]]

Output = [[0],
          [1],
          [0],
          [0],
          [1],
          [0],
          [1],
          [0]]

Risultato = []

for n,i in enumerate(Output):
    if i[0] == 1:
        Risultato.append("A" if Input[n][0] else "a")
        Risultato.append("*")
        Risultato.append("B" if Input[n][1] else "b")
        Risultato.append("*")
        Risultato.append("C" if Input[n][2] else "c")

print("Input")
print(Input)
print("Output")
print(Output)
print("Risultato")
print(Risultato)