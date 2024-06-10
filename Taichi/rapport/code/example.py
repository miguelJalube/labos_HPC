import taichi as ti
import taichi.math as tm

# Initialisation de l'environnement Taichi
#   ti.init(arch=ti.cpu) # Pour utiliser le CPU
ti.init(arch=ti.gpu)

# Définition d'une Taichi field
#   Cette fonction permet de définir une fenetre de pixels dont 
#   la taille est définie par les paramètres height et width puis
#   de définir un type de donnée pour chaque pixel (ici float)
n = 1000
pixels = ti.field(dtype=float, shape=(n*2, n))

# Définition de la fonction Taichi
@ti.func
def complex_sqr(z):
    return tm.vec2([z[0] ** 2 - z[1] ** 2, z[1] * z[0] * 2])

@ti.kernel
def render(t: float):
    for i, j in pixels:
        c = tm.vec2([-0.8, ti.sin(t) * 0.2])
        z = tm.vec2([i / n - 1, j / n - 0.5]) * 2
        iterations = 0
        while z.norm() < 20 and iterations < 50:
            z = complex_sqr(z) + c
            iterations += 1
        pixels[i, j] = 1 - iterations * 0.02                                                        
                                                                                                               
                                                                                                               
def main():                                                                                                    
    gui = ti.GUI("Window", res=(n*2, n))                                                                  
    t = 0.0                                                                                                    
    while not gui.get_event(ti.GUI.ESCAPE, ti.GUI.EXIT):                                                       
        render(t)                                                                                               
        t += 0.03                                                                                   
        gui.set_image(pixels)                                                                                  
        gui.show()                                                                                             
                                                                                                               
                                                                                                               
if __name__ == "__main__":                                                                                     
    main()