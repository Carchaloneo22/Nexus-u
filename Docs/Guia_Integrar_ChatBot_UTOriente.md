# Guia: Integrar ChatBot UT Oriente al Proyecto

## ARCHIVOS GENERADOS

| Archivo | Ruta | Formato |
|---|---|---|
| FAQ Completo JSON | `Content/Data/ChatBot/FAQ_IS_Completo_UTOriente.json` | JSON con array de 56 FAQs |
| FAQ DataTable CSV | `Content/Data/ChatBot/DT_FAQ_IS_Import_UTOriente.csv` | CSV listo para importar a Unreal DataTable |

---

## PASO 1: IMPORTAR CSV COMO DATATABLE (Recomendado)

### Opcion A: Usar DataTable existente

1. Abre **Unreal Editor**.
2. Ve a `Content/Data/ChatBot/`.
3. Click derecho → **Import to `/Game/Data/ChatBot/`**.
4. Selecciona el archivo `DT_FAQ_IS_Import_UTOriente.csv`.
5. En el diálogo de importación:
   - **Row Structure**: Selecciona o crea un Struct que coincida con las columnas:
     - `Keywords` (FString)
     - `Response` (FString)
     - `Category` (FString)
   - Si el Struct actual tiene `Keywords` como Array de Strings, el CSV no importará arrays directamente. En ese caso usa la **Opción B**.
6. Nombra el DataTable: `DT_FAQ_IS_UTOriente`.
7. Click **Import**.

### Opcion B: Si el Struct tiene Array de Keywords

Si el Struct `FS_ChatBotFAQ` tiene `Keywords` como `Array<FString>`, el CSV no puede importar arrays fácilmente. En ese caso:

1. **Mantener el CSV** para referencia manual.
2. **Crear un Blueprint Function Library** o modificar el chatbot existente para que lea el **JSON** en lugar del DataTable.

---

## PASO 2: CONFIGURAR CHATBOT PARA LEER JSON (Mas flexible)

El JSON es más fácil de manejar si el chatbot actual usa Blueprint.

### Blueprint Setup:

1. Abre el Blueprint que maneja el chatbot (`BP_ChatTerminal_IS` o similar).
2. En el Event Graph, busca la sección donde carga las FAQs.
3. Reemplaza la referencia al DataTable/JSON antiguo por el nuevo archivo:
   ```
   Get File Path: Content/Data/ChatBot/FAQ_IS_Completo_UTOriente.json
   |
   Read File (node) → Returns String
   |
   Json Parse (node) → Returns Json Object
   |
   Get Field (field name = "faqs") → Returns Json Array
   |
   For Each Loop (Json Array)
      |
      Get Field ("keywords") → Array de Strings
      Get Field ("response") → String
      Get Field ("category") → String
      |
      Almacenar en array de structs o variables
   ```

4. Guardar el array en una variable de tipo **Array** de tu Struct `FS_ChatBotFAQ`.

---

## PASO 3: LOGICA DE RESPUESTA (Keyword Matching)

### Funcion: BuscarRespuesta(InputText)

```
Input: Texto del jugador (String)

Steps:
1. Convertir InputText a MINUSCULAS (ToLower)
2. For Each (FAQ en Array de FAQs):
      |
      For Each (Keyword en FAQ.Keywords):
          |
          Contains (InputText contiene Keyword?)
              |
              Si True: Return FAQ.Response + " [" + FAQ.Category + "]"
3. Si ninguna coincide:
   Return "No entendí bien. Puedo ayudarte con: precio, semestres, materias, requisitos, plan de estudios, contacto y perfil del egresado. ¿Qué te interesa?"
```

### Blueprint Nodes exactos:

```
Event OnSendMessage (TextoUsuario)
  |
  ├──→ ToLower (TextoUsuario) → TextoLower
  |
  ├──→ Set Variable "RespuestaEncontrada" = False
  |
  ├──→ For Each Loop (FAQ_Array)
  |     |
  |     ├──→ For Each Loop (FAQ.Keywords)
  |     |     |
  |     |     ├──→ Contains (TextoLower contiene Keyword?)
  |     |     |     |
  |     |     |     ├── True:
  |     |     |     |   ├──→ Set RespuestaEncontrada = True
  |     |     |     |   ├──→ Set Text (ChatOutput) = FAQ.Response
  |     |     |     |   └──→ Break Loop
  |     |     |     |
  |     |     |     └── False: Continue
  |     |     |
  |     |     └── Branch (RespuestaEncontrada?) → True: Break outer loop
  |     |
  |     └── (loop continúa)
  |
  ├──→ Branch (RespuestaEncontrada?)
  |     ├── True: (ya seteado arriba)
  |     └── False:
  |         └──→ Set Text (ChatOutput) = "No entendí bien. Puedo ayudarte con: precio, semestres, materias, plan de estudios, requisitos, contacto y perfil. ¿Qué te interesa?"
```

---

## PASO 4: CONECTAR AL WIDGET WBP_ChatBot_IS

Si el widget `WBP_ChatBot_IS` ya existe:

1. **Abrir Widget** → Designer Tab.
2. Verificar que tenga:
   - `EditableTextBox` (input del usuario) → nombre: `TXT_Input`
   - `MultiLineEditableText` o `ScrollBox` + `TextBlock` (output del bot) → nombre: `TXT_Output`
   - `Button` Enviar → nombre: `BTN_Enviar`

3. **Event Graph** del Widget:

```
Event Construct
  |
  └──→ Llamar a "CargarFAQ_JSON" (función custom en BP_ChatTerminal_IS o GameInstance)

OnTextCommitted (TXT_Input)
  |
  ├──→ Get Text (TXT_Input) → TextoUsuario
  |
  ├──→ Llamar "BuscarRespuesta" con TextoUsuario
  |     Return Value: RespuestaBot
  |
  ├──→ Append al historial:
  |     TXT_Output += "\nTú: " + TextoUsuario
  |     TXT_Output += "\nBot: " + RespuestaBot
  |
  ├──→ Set Text (TXT_Input) = "" (limpiar)
  |
  └──→ Scroll To End (ScrollBox)
```

4. **Binding**: Si `WBP_ChatBot_IS` está abierto desde `BP_ChatTerminal_IS`, asegurar que el PlayerController tenga input UI Only cuando el widget esté abierto.

---

## CONTENIDO DEL CHATBOT (56 FAQs)

### Categorias cubiertas:

| Categoria | Num FAQs | Temas |
|---|---|---|
| Saludos | 3 | Hola, adiós, gracias |
| Ayuda | 3 | Opciones, no entiendo, help |
| General | 12 | Descripción carrera, universidad, SNIES, resolución, duración, créditos, modalidad, título, ventajas, ética, comparación con sistemas, certificado |
| Precio | 4 | Valor semestre, total, becas/financiación |
| Contacto | 4 | Teléfono, WhatsApp, correo, dirección |
| Requisitos | 5 | Inscripción, Saber 11, requisitos técnicos |
| Plan | 22 | Cada semestre detallado, materias específicas (programación, BD, web, móvil, redes, seguridad, UI/UX, inglés, electivas) |

### Preguntas clave que responde:

- ¿Cuánto cuesta? → COP $2.200.000/semestre
- ¿Cuánto dura? → 8 semestres (4 años)
- ¿Cuál es el SNIES? → 118074
- ¿Qué materias veo en el semestre X? → Lista detallada de las 6 materias
- ¿Qué necesito para inscribirme? → Diploma, ICFES, cédula, foto, EPS
- ¿Es virtual? → Sí, 100%
- ¿Qué tecnologías veo? → Big Data, IoT, IA, Web, Móvil
- ¿Por qué estudiar aquí? → Virtual, Big Data, IoT, UI/UX, inglés, ética
- ¿Qué recibo al graduarme? → Ingeniero(a) de Software

---

## NOTAS TÉCNICAS

### JSON vs DataTable

| Opción | Pros | Contras |
|---|---|---|
| **JSON** | Fácil de editar fuera de Unreal, soporta arrays nativamente, no requiere importación | Necesita parseo en Blueprint (Read File → Json Parse) |
| **DataTable** | Integrado con UE, visor nativo, type-safe | No importa arrays desde CSV fácilmente, requiere reimportar al cambiar |

**Recomendación para este proyecto:** Usar el **JSON** directamente en Blueprint. Es más rápido de modificar y el parseo de JSON es nativo en Blueprint (nodos `Read File`, `Json Parse`, `Get Field`).

### Performance

- 56 FAQs es negligible en memoria (~20KB).
- El keyword matching lineal es instantáneo para este volumen.
- No requiere optimización adicional.

---

## CHECKLIST DE INTEGRACION

- [ ] Copiar `FAQ_IS_Completo_UTOriente.json` a `Content/Data/ChatBot/` (ya está ahí)
- [ ] Abrir `BP_ChatTerminal_IS` o BP que maneja el chatbot
- [ ] Reemplazar referencia al JSON/DT antiguo por `FAQ_IS_Completo_UTOriente.json`
- [ ] Implementar función `CargarFAQ` (Read File → Json Parse → Get Field "faqs" → Store Array)
- [ ] Implementar función `BuscarRespuesta(Texto)` (ToLower → ForEach Keywords → Contains → Return Response)
- [ ] Conectar `BTN_Enviar` en `WBP_ChatBot_IS` a `BuscarRespuesta`
- [ ] Mostrar respuesta en `TXT_Output`
- [ ] Probar en PIE: escribir "precio" → debe responder COP $2.200.000
- [ ] Probar "primer semestre" → debe listar las 6 materias
- [ ] Probar "hola" → debe saludar
- [ ] Probar "adsfjkl" → debe dar mensaje de ayuda/fallback

---

## ARCHIVOS ADICIONALES RECOMENDADOS

Si quieres expandir el chatbot más adelante:

1. **Guía de materias por semestre** (imagen o PDF) → mostrar en UI como "Ver plan de estudios".
2. **Videos explicativos** → integrar MediaPlayer en widget para reproducir video promocional.
3. **Formulario de contacto** → widget con campos Nombre, Correo, Pregunta que envíe a un endpoint (requiere backend).

---

## PROXIMO PASO DESPUES DEL CHATBOT

Cuando el chatbot funcione → continuar con minijuegos Isla 2:
1. Minijuego Redes (conexión modem→router→PC)
2. Minijuego PSeInt (quiz pseudocódigo)
3. Minijuego Comandos/Grid (mover bloque en cuadrícula)
