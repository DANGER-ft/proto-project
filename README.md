protoType

EN | RU

RU

Типа лол, сидел скучал и тут решил сделать пакости. В итоге: −личная жизнь, +«а где я end не поставил-то??»

Что это?

Это «операционная система» (это не так, но этим словосочетанием я описываю десяток строк того, что это есть, и если не придираться, то это правда), написанная для семейства плат esp32, а конкретно esp32s3, для работы с дисплеем, обеспечивая функционал простейшей ОС. Внутри — менеджер задач, ядро (по названию, но не по определению, но не суть), setup-скрипт, подгружающий все необходимые файлы и крутящий файловый менеджер. В дополнение пока что 1 программа. Если по-реальному, то это нечто, пытающееся назвать себя ОС, но бьющееся мордой в пиз... т.е. суровую реальность. Внебрачный сын unix и dos. АВТОР ДЕБИЛ И НЕ ВИДЕЛ НИ РАЗУ НИЧЕГО СЛОЖНЕЕ HTML-САЙТОВ! Родной язык — cpp, но натянут lua. Вычислительная мощность ~~1 муха. Ну а что вы хотели на плате за 350 рублей (~5 долларов)? Вот и оправдание мне, грешному. И не говорите, мол, какие-нибудь ямахи были слабее, а вы там на них... я в домике.

Возможности

Эм... ну... есть, присутствуют.
Реализовано приложений на момент последнего коммита:

· файловый менеджер

Также можете вырезать всё из data, заменив на свои .lua, но пропишите в main запускаемый файл (изначально run.lua в корне) и используйте биндинги. Но тут за отладку не отвечаю (в особенности за lvgl; если что, img выводить через кастомный фанк, а не стандартные методы). В общем, при таком сценарии смотрите .h и .cpp. Также мой набор скриптов крайне сырой, не для шир-потреба (я не винду пишу, лярд проверок мне писать смысла нет), а скорее как база для разработки или для продолжения работы.

Сборка

PlatformIO. Все зависимости лежат в репе, так что ничего качать не надо (большинство патченное, к слову).
Загрузка стандартная: pio run --target upload, pio --target uploadfs. В дальнейшем для оптимизации файлы можно загружать через ftp, НО ТОЛЬКО В 1 ПОТОК (он был отлажен и проверен до конца, используйте какой-нибудь FileZilla с соответствующими настройками). Для этого надо вернуть управление в loop (выйти из всех while или for и завершить работу скрипта). В моей прошивке это выполняется через такой пункт в gui, как pelzigerFemj (не спрашивайте, все названия в нём — это заглушки).

Смысл в этой хрени

Нету. 0. null. nil. !смысл. ~смысл. -смысл. смысл * (-1). -(смысл ^ [переведите в utf-16, сложите и обратно в десятичку]). Но можете использовать кучу реализованных решений (30% из которых я у кого-то украл... но тоже с гитхаба).

Железо

· плата esp32 s3 devkitc1 n16r8 (надеюсь, не перепутал буквы: 16 flash, 8 psram. Формально можно любую из семейства esp32 s3-х с поддержкой usb-хоста, но не меньше 2 МБ psram желательно (в куче sram остаётся ~50%)).
· дисплей 128x160 st7789s. Берите любой, НО всё подогнано под мой размер.
· любая клавиатура по usb с переходником в штатный (у меня 2) / на внешний порт. При использовании той же платы, что и у меня, запаяйте перемычку на нижней стороне, иначе не пойдёт 5 В на usb, но тогда подключать сразу оба порта к пк нельзя (вроде как будет салют) — это если штатный порт. Если внешний, то запаять другую, на подачу 5 В на соответствующий пин. Без клавиатуры моя система не работает, только кастомные скрипты.

Скриншоты

...

EN

So lol, I was bored and decided to cause some trouble. Result: −personal life, +"wait, where did I forget the end??"

What is this?

It's an "operating system" (it isn't, but that's the phrase I use to describe the dozen or so lines of what this actually is — and if you don't nitpick, it's technically true), written for the esp32 family of boards, specifically the esp32s3, to drive a display and provide the functionality of the most basic OS. Under the hood there's a task manager, a "kernel" (kernel by name, not by definition — but that's beside the point), and a setup script that pulls in all the necessary files and runs a file manager. As a bonus, there's currently exactly 1 program. If we're being honest, it's something that tries to call itself an OS but keeps faceplanting into the puss... i.e. harsh reality. The bastard child of unix and dos. THE AUTHOR IS AN IDIOT WHO HAS NEVER SEEN ANYTHING MORE COMPLEX THAN HTML PAGES! Native language is cpp, with lua stretched over it. Computing power ~~1 fly. Well, what did you expect from a 350-ruble (~$5) board? There's my excuse, sinner that I am. And don't come at me with "but some yamahas were weaker and you did stuff on those..." — I'm in my safe space.

Features

Um... well... they exist, they're present.
Apps implemented as of the latest commit:

· file manager

You're also free to gut everything in data and swap in your own .lua files — just point main at the startup file (run.lua in the root by default) and use the bindings. I don't vouch for debugging in that scenario though (especially with lvgl; if anything, render img through the custom func, not the standard methods). So if you go down that route, look at the .h and .cpp files. Also, my script set is extremely raw and not meant for wide consumption (I'm not writing Windows here — no point in me adding a billion sanity checks), more of a base for development or for picking up where I left off.

Build

PlatformIO. All dependencies are in the repo, so nothing needs to be downloaded (most of them are patched, by the way).
Uploading is standard: pio run --target upload, pio --target uploadfs. Later on, for optimization, you can upload files over ftp — BUT ONLY IN 1 STREAM (that path was fully debugged and verified, so use something like FileZilla with the appropriate settings). To do that, you need to hand control back to loop (break out of any while or for and let the script finish). In my firmware this is done through a gui item called pelzigerFemj (don't ask — all the names in it are placeholders).

Point of this crap

There isn't one. 0. null. nil. !point. ~point. -point. point * (-1). -(point ^ [convert to utf-16, add them up, convert back to decimal]). That said, you can still make use of a bunch of implemented solutions (30% of which I stole from someone... but also from github).

Hardware

· esp32 s3 devkitc1 n16r8 board (hope I didn't mix up the letters: 16 flash, 8 psram. Technically any board from the esp32 s3 family with usb host support will do, but ideally no less than 2 MB of psram (about ~50% is left in the sram heap)).
· 128x160 st7789s display. Take any, BUT everything is tailored to my size.
· any usb keyboard with an adapter into the onboard port (I have 2) / to the external port. If you're using the same board as me, solder the jumper on the bottom side, otherwise 5 V won't reach usb — but then you can't plug both ports into a pc at the same time (apparently there'll be fireworks). That's for the onboard port. If it's the external one, solder the other jumper to feed 5 V to the corresponding pin. Without a keyboard my system doesn't work — only custom scripts do.

Screenshots

...

License

MIT
