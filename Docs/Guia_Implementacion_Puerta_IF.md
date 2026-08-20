# Guia de Implementacion - Minijuego Puerta (Blueprint Visual IF)

## RESUMEN
Minijuego tutorial de condicional `IF` para Isla 1 (Spawn).
Usuario aprende: condicion → rama True/False → accion (abrir puerta).
Al completar: puerta se abre + puente aparece para Isla 2.

---

## PARTE 1: MEJORAR WBP_PUZZLE_IF (Widget)

### A. Jerarquia (Panel de widgets) - Modificaciones

Abre `Content/UI/Puzzles/WBP_Puzzle_IF`.

**1. Reorganizar jerarquia actual:**
```
[Canvas Panel] (raiz)
├── TXT_Titulo "Ejemplo" → Renombrar a "TXT_Titulo"
├── TXT_Resultado (texto explicacion, debajo del titulo)
├── [Canvas Panel] NodoInicio_Container
│   ├── [Border] (fondo morado/claro)
│   ├── TXT_NodoInicio "Iniciar"
│   └── [Image] FlechaInicio → Branch
├── [Canvas Panel] NodoCondicion_Container
│   ├── [Border]
│   ├── TXT_NodoCondicion "Branch IF"
│   ├── CHK_Condicion (ComboBox o Toggle unico)
│   └── TXT_CondicionLabel "¿La puerta está cerrada?"
├── [Canvas Panel] NodoRama_Container
│   ├── [Border] RamaTrue_Container
│   │   ├── TXT_True "True → Abrir"
│   │   └── [Image] FlechaTrue
│   └── [Border] RamaFalse_Container
│       ├── TXT_False "False → Cerrar"
│       └── [Image] FlechaFalse
├── [Canvas Panel] NodoAccion_Container
│   ├── [Border]
│   ├── TXT_Accion "Abrir Puerta"
│   └── BTN_AbrirPuerta (deshabilitado por defecto)
├── BTN_Verificar (centrado abajo)
└── BTN_Cerrar (esquina inferior derecha)
```

**2. Eliminaciones:**
- Eliminar los `[Canvas Panel]` sin nombre que estan vacios.
- Eliminar `CHK_EstaAbierta?` y `CHK_EstaCerrada?` (reemplazar por control unico).
- Eliminar nodo duplicado "Cerrar Puerta" (solo dejamos "Abrir Puerta" como accion positiva del tutorial).

**3. Control de condicion recomendado:**
- Agregar `ComboBoxString` llamado `CMB_Condicion`.
- Opciones:
  - `"La puerta está cerrada"` (Default)
  - `"La puerta está abierta"`
- O usar `CheckBox` unico: `"¿La puerta está cerrada?"` (marcado = True).

### B. Diseño Visual (Designer Tab)

**Posiciones recomendadas (en Canvas Panel 1920x1080):**

| Widget | Posicion X,Y | Tamano | Nota |
|---|---|---|---|
| TXT_Titulo | 960, 50 | 400x60 | Anchor Top Center, Font Size 28, Bold, Color Blanco |
| TXT_Resultado | 960, 120 | 800x40 | Anchor Top Center, Font Size 18, Color Amarillo (#FFD700), Inicialmente vacio |
| NodoInicio_Container | 300, 250 | 200x80 | Border Background #4A148C (morado oscuro) |
| FlechaInicio (Image) | 500, 270 | 100x20 | Flecha derecha gris |
| NodoCondicion_Container | 650, 200 | 280x150 | Border Background #1A237E (azul oscuro), centrado |
| FlechaTrue (Image) | 930, 250 | 80x20 | Flecha derecha gris |
| FlechaFalse (Image) | 930, 350 | 80x20 | Flecha derecha gris |
| RamaTrue_Container | 1050, 220 | 180x60 | Border Background #2E7D32 (verde oscuro) |
| RamaFalse_Container | 1050, 340 | 180x60 | Border Background #C62828 (rojo oscuro) |
| NodoAccion_Container | 1300, 220 | 220x80 | Border Background #1565C0 (azul), BTN_AbrirPuerta dentro |
| BTN_Verificar | 960, 500 | 200x50 | Texto "Verificar", Color #FF6F00 (naranja) |
| BTN_Cerrar | 1750, 1000 | 120x40 | Texto "Cerrar" |

**Colores de texto en nodos:** Blanco (#FFFFFF).
**Fuentes:** Roboto o default UE.

### C. Event Graph - Nodos Exactos

#### 1. Event Construct
```
Event Construct
  |
  ├──→ Set IsEnabled (Target = BTN_AbrirPuerta) → False
  |
  ├──→ Set Visibility (Target = FlechaInicio) → Hidden
  ├──→ Set Visibility (Target = FlechaTrue) → Hidden
  ├──→ Set Visibility (Target = FlechaFalse) → Hidden
  |
  └──→ Set Text (Target = TXT_Resultado) → "Selecciona la condición y presiona Verificar"
```

#### 2. OnClicked BTN_Verificar
```
OnClicked (BTN_Verificar)
  |
  ├──→ Get Selected Option (CMB_Condicion)  [o leer CheckBox]
  |
  ├──→ Branch Condition: == "La puerta está cerrada"
      |
      ├── True (condicion IF es TRUE → abrir puerta):
      |   ├──→ Set Color and Opacity (NodoCondicion_Container) → Border #66BB6A (verde claro)
      |   ├──→ Set Color and Opacity (RamaTrue_Container) → Border #43A047 (verde)
      |   ├──→ Set Visibility (FlechaInicio) → Visible
      |   ├──→ Set Visibility (FlechaTrue) → Visible
      |   ├──→ Set Visibility (FlechaFalse) → Hidden
      |   ├──→ Set Text (TXT_Resultado) → "¡Correcto! La condición es TRUE porque la puerta está cerrada. El flujo va por la rama 'True' y ejecuta 'Abrir Puerta'."
      |   └──→ Set IsEnabled (BTN_AbrirPuerta) → True
      |
      └── False (puerta abierta → rama False):
          ├──→ Set Color and Opacity (NodoCondicion_Container) → Border #EF5350 (rojo claro)
          ├──→ Set Color and Opacity (RamaFalse_Container) → Border #E53935 (rojo)
          ├──→ Set Visibility (FlechaInicio) → Visible
          ├──→ Set Visibility (FlechaTrue) → Hidden
          ├──→ Set Visibility (FlechaFalse) → Visible
          ├──→ Set Text (TXT_Resultado) → "La condición es FALSE porque la puerta ya está abierta. El flujo va por la rama 'False'. Intenta cambiar la condición."
          └──→ Set IsEnabled (BTN_AbrirPuerta) → False
```

#### 3. OnClicked BTN_AbrirPuerta
```
OnClicked (BTN_AbrirPuerta)
  |
  ├──→ Call "OnPuzzleSolved" (Event Dispatcher)
  |
  ├──→ Set Text (TXT_Resultado) → "¡Puerta abierta! Ahora el puente ha aparecido. Puedes cruzar a la Isla 2."
  |
  ├──→ Delay 2.0 segundos
  |
  └──→ Remove from Parent
```

#### 4. Event Dispatcher: OnPuzzleSolved
- Crear Event Dispatcher en WBP llamado `OnPuzzleSolved`.
- No tiene parametros.

#### 5. OnClicked BTN_Cerrar
```
OnClicked (BTN_Cerrar)
  |
  └──→ Remove from Parent
```

---

## PARTE 2: CREAR BP_PUERTA_IF (Actor Puerta)

### A. Crear Blueprint
1. Click derecho en `Content/World/` → Blueprint Class → Actor.
2. Nombre: `BP_Puerta_IF`.
3. Abrir.

### B. Componentes
```
[Root] DefaultSceneRoot
├── SM_Puerta (Static Mesh Component)
│   └── Mesh: elegir un mesh rectangular de puerta (o usar Cube escalado)
│   └── Transform: Scale (0.1, 1.5, 2.0) aprox
└── Colision (Box Collision, opcional)
```

### C. Event Graph

#### Custom Event: AbrirPuerta
```
Custom Event "AbrirPuerta"
  |
  ├──→ Set World Rotation (Target = SM_Puerta)
  │     New Rotation: 0, 90, 0  (abre hacia un lado)
  │     Sweep: true
  │     Teleport: false
  │     (O usar Timeline para animacion suave)
  |
  └──→ Play Sound 2D (opcional: sonido de puerta)
```

**Alternativa con Timeline (animacion suave):**
```
Custom Event "AbrirPuerta"
  |
  ├──→ Play (Timeline "TL_AbrirPuerta")
  |
Timeline "TL_AbrirPuerta":
  - Track: Float Track "PuertaAngle"
  - Keys: (0, 0) → (2.0, 90)
  - Set New Time (en update):
      → Set Relative Rotation (SM_Puerta) = 0, PuertaAngle, 0
```

---

## PARTE 3: CREAR BP_PUENTE_IF (Actor Puente)

### A. Crear Blueprint
1. Click derecho en `Content/World/` → Blueprint Class → Actor.
2. Nombre: `BP_Puente_IF`.
3. Abrir.

### B. Componentes
```
[Root] DefaultSceneRoot
└── SM_Puente (Static Mesh Component)
    └── Mesh: Puente1 o Puente2 de Content/World/Isla1/
    └── Inicialmente: Visible = true PERO Scale Z = 0 (invisible) o Hidden in Game = true
```

### C. Event Graph

#### Custom Event: MostrarPuente
```
Custom Event "MostrarPuente"
  |
  ├──→ Set Actor Hidden in Game → False  (si estaba hidden)
  |
  O alternativa con escalado:
  ├──→ Set World Scale 3D → (1, 1, 0.1)  (aparece delgado)
  ├──→ Timeline: Scale Z 0.1 → 1.0 en 1.5 seg
  └──→ Set World Scale 3D (en update) → (1, 1, TimelineValue)
```

---

## PARTE 4: CONECTAR BP_PUZZLETERMINAL_IF

### A. Abrir BP_PuzzleTerminal_IF

#### Variables a agregar:
| Nombre | Tipo | Default | Explicacion |
|---|---|---|---|
| `PuertaRef` | `BP_Puerta_IF` Object Reference | None | Referencia a la puerta en el mundo |
| `PuenteRef` | `BP_Puente_IF` Object Reference | None | Referencia al puente |
| `WidgetClass` | `WBP_Puzzle_IF` Class Reference | `WBP_Puzzle_IF` | Clase del widget |
| `ActiveWidget` | `WBP_Puzzle_IF` Object Reference | None | Instancia actual del widget |

#### Event Graph - Interaccion

```
Event BeginPlay
  |
  └──→ (nada especial, solo preparar)

OnActorBeginOverlap (InteractionSphere)
  |
  ├──→ Cast to BP_ThirdPersonCharacter (Other Actor)
  |
  └──→ (opcional: mostrar prompt "Presiona E para interactuar")

// Tecla de interaccion (o usar Interact Input Action del player)
Event Interact (o llamado desde PlayerController)
  |
  ├──→ Is Valid (ActiveWidget)?
  |     ├── True: return (ya abierto)
  |     └── False: continuar
  |
  ├──→ Create Widget (Widget Class = WidgetClass)
  |     Return Value → ActiveWidget
  |
  ├──→ Add to Viewport (Target = ActiveWidget)
  |
  ├──→ Set Input Mode UI Only
  |     In Widget to Focus: ActiveWidget
  |
  ├──→ Show Mouse Cursor (PlayerController) → True
  |
  └──→ Bind Event to OnPuzzleSolved (Target = ActiveWidget)
        Event: Custom Event "HandlePuzzleSolved"
```

#### Custom Event: HandlePuzzleSolved
```
Custom Event "HandlePuzzleSolved"
  |
  ├──→ Set Input Mode Game Only
  |
  ├──→ Show Mouse Cursor (PlayerController) → False
  |
  ├──→ AbrirPuerta (Target = PuertaRef)
  |
  ├──→ MostrarPuente (Target = PuenteRef)
  |
  ├──→ Delay 3.0 segundos
  |
  └──→ Set ActiveWidget → None (limpiar referencia)
```

### B. Colocar en el mapa (islas.umap)

1. Arrastrar `BP_PuzzleTerminal_IF` a Isla 1.
2. Arrastrar `BP_Puerta_IF` frente al terminal.
3. Arrastrar `BP_Puente_IF` cerca de la puerta (inicialmente hidden o scale 0).
4. Seleccionar `BP_PuzzleTerminal_IF` → Details Panel → Default:
   - `PuertaRef` → seleccionar la instancia de `BP_Puerta_IF` del mapa.
   - `PuenteRef` → seleccionar la instancia de `BP_Puente_IF` del mapa.

---

## PARTE 5: AJUSTES DEL PLAYER

### A. PlayerController o Character

Asegurar que el input `IA_Interact` (ya existe) llame a interactuar con el terminal.

En `BP_ThirdPersonCharacter` o `BP_ThirdPersonPlayerController`:

```
OnPressed IA_Interact
  |
  ├──→ Line Trace o Sphere Trace (buscar actor interactuable)
  |
  ├──→ If Hit Actor implements Interface BPI_Interact (o es BP_PuzzleTerminal_IF)
  |
  └──→ Call "Interact" event en el actor
```

**Nota:** Si ya tienes un sistema de interaccion (como el chat terminal), reusar la misma logica.

---

## CHECKLIST DE PRUEBA

- [ ] WBP_Puzzle_IF se abre al interactuar con terminal
- [ ] Boton "Verificar" resalta rama correcta segun condicion
- [ ] Boton "Abrir Puerta" solo se habilita si condicion es TRUE
- [ ] Clic en "Abrir Puerta" cierra widget y abre puerta 3D
- [ ] Puente aparece despues de abrir puerta
- [ ] Player puede cruzar puente a Isla 2
- [ ] Boton "Cerrar" funciona sin abrir puerta
- [ ] No hay errores en Output Log

---

## PROXIMOS PASOS DESPUES DE ESTE MINIJUEGO

1. **Chatbot UT Oriente** → Reemplazar `DT_FAQ_IS` con nuevo contenido.
2. **Minijuego Redes** (Isla 2) → WBP con drag & drop de modem→router→PC.
3. **Minijuego PSeInt** (Isla 2) → WBP quiz de pseudocodigo.
4. **Minijuego Comandos/Grid** (Isla 2) → WBP grid 5x5 con botones de direccion.
