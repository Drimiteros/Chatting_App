#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <string>

using namespace std;
using namespace sf;

int main()
{
    string IP;
    int port;
    IpAddress ip = IpAddress::getLocalAddress();
    IpAddress ip2 = IpAddress::getPublicAddress();
    TcpSocket socket;
    TcpListener listener;
    char connectionType;

    cout << " Type 's' for Server or 'c' for Client >> ";
    cin >> connectionType;
    cout << "" << endl;
    system("cls");
    cout << " Your IP address: " << ip << endl;
    cout << " Your public IP address: " << ip2 << endl;
    cout << " Enter the IP Adress of the PC that you wish to connect to or host >> ";
    cin >> IP;
    system("cls");
    cout << " Enter a port number (Make sure to be the same as the Server's) >> ";
    cin >> port;
    system("cls");
    cout << " Waiting for connections... ";

    //SERVER
    if (connectionType == 's')
    {
        listener.listen(port);
        listener.accept(socket);
        system("cls");
        cout << "Connection to client establised" << endl;
    }

    //CLIENT
    else if (connectionType == 'c')
    {
        socket.connect(IP, port);
        system("cls");
        cout << "Connection to server " << IP << " establised" << endl;
    }

    //Backround
    Texture backroundTexture;
    backroundTexture.loadFromFile("Images/backround.png");
    Sprite backround(backroundTexture);

    //SEND
    Texture sendtexture;
    sendtexture.loadFromFile("Images/send.png");
    Sprite send;
    send.setTexture(sendtexture);
    send.setScale(0.3, 0.3);

    //SOUND
    SoundBuffer buffer;
    buffer.loadFromFile("Sounds/sound.wav");
    Sound sound;
    sound.setBuffer(buffer);
    Clock soundClock;

    //CURSOR
    RectangleShape cursor(Vector2f(10, 10));

    //Font
    Font font;
    if (!font.loadFromFile("Fonts/typewcond_demi.otf"))
    {
        cout << "failed to load font" << endl;
    }
    char ch = '\b';
    Text P1text;
    P1text.setFont(font);
    P1text.setFillColor(Color::White);
    P1text.setPosition(25, 650);
    string player1Input;
    string subplayer1input;
    string smallText1;
    Text text1;
    text1.setFillColor(Color::Yellow);
    text1.setFont(font);
    text1.setPosition(15, 150);
    Clock eraceClock;

    socket.setBlocking(false);

    RenderWindow window(VideoMode(700, 700), "SC");
    Event eve;

    string stream[10000];
    int pos = 0;

    int x = 580, y = 40;
    RectangleShape area(Vector2f(x, y));
    area.setFillColor(Color::Black);
    area.setOutlineColor(Color::White);
    area.setOutlineThickness(2);
    area.setPosition(10, 650);
    Clock clock;

    while (window.isOpen())
    {
        while (window.pollEvent(eve))
        {
            if (eve.type == Event::Closed)
            {
                window.close();
            }

            if (eve.type == Event::TextEntered)
            {
                //User text
                if (player1Input.size() <= 162 && !Keyboard::isKeyPressed(Keyboard::BackSpace))
                {
                    player1Input += eve.text.unicode;
                    P1text.setString(player1Input);
                }
            }
        }

        //CURSOR
        cursor.setPosition(Mouse::getPosition(window).x, Mouse::getPosition(window).y);

        //Chat mechanics:
        if (player1Input.size() == 40 || player1Input.size() == 81 || player1Input.size() == 122
            || Keyboard::isKeyPressed(Keyboard::Enter))
        {
            if (clock.getElapsedTime().asSeconds() > 0.1)
            {
                clock.restart();
                player1Input = player1Input + "\n";
                y = y + 40;
                area.setPosition(area.getPosition().x, area.getPosition().y - 40);
                P1text.setPosition(P1text.getPosition().x, P1text.getPosition().y - 40);
            }
        }
        area.setSize(Vector2f(x, y));

        if (cursor.getGlobalBounds().intersects(send.getGlobalBounds()))
        {
            send.setScale(0.33, 0.33);
            send.setPosition(600, 605);
        }
        else
        {
            send.setScale(0.3, 0.3);
            send.setPosition(605, 610);
        }

        Packet packet;
        if (cursor.getGlobalBounds().intersects(send.getGlobalBounds()) && Mouse::isButtonPressed(Mouse::Left))
        {
            pos++;
            stream[pos] = player1Input;
            packet << player1Input;
            socket.send(packet);
        }
        if (Keyboard::isKeyPressed(Keyboard::BackSpace))
        {
            if (Keyboard::isKeyPressed(Keyboard::LShift))
            {
                player1Input = "";
                P1text.setString("");
            }

            if (eraceClock.getElapsedTime().asSeconds() > 0.2)
            {
                subplayer1input = player1Input.substr(0, player1Input.length() - 1);
                player1Input = subplayer1input;
                P1text.setString(subplayer1input);

                y = 40;
                area.setPosition(10, 650);
                P1text.setPosition(25, 650);

                eraceClock.restart();
            }
        }
        socket.receive(packet);
        if (packet >> smallText1)
        {
            soundClock.restart();
            text1.setString(">> " + smallText1);
            if(soundClock.getElapsedTime().asSeconds() > 0 && soundClock.getElapsedTime().asSeconds() < 0.400)
                sound.play();
        }

        window.clear();
        window.draw(backround);
        window.draw(area);
        window.draw(send);
        window.draw(P1text);
        window.draw(text1);
        window.display();
    }
}
   /*int main()
   {
       IpAddress ip = "192.168.1.9";
       TcpSocket socket;
       char connectionType;

       cout << "(s) for Server, (c) for Client: ";
       cin >> connectionType;

       if (connectionType == 's')
       {
           TcpListener listener;
           listener.listen(2000);
           listener.accept(socket);
       }
       else
           socket.connect(ip, 2000);
       RectangleShape rect1, rect2, rect3;
       rect1.setSize(Vector2f(20, 20));
       rect1.setFillColor(Color::White);
       rect2.setSize(Vector2f(20, 20));
       rect2.setFillColor(Color::Blue);
       rect3.setSize(Vector2f(20, 20));
       rect3.setFillColor(Color::Red);

       RenderWindow window(VideoMode(700, 700), "packets");
       Event eve;
       Vector2f p1Position, p2Position, p3Position;
       socket.setBlocking(false);
       bool update;

       while (window.isOpen())
       {
           while (window.pollEvent(eve))
           {

               if (eve.type == Event::GainedFocus)
                   update = true;
               else if (eve.type == Event::LostFocus)
                   update = false;
           }

           p1Position = rect1.getPosition();
           p2Position = rect2.getPosition();
           p3Position = rect3.getPosition();

           if (update == true)
           {
               if (Keyboard::isKeyPressed(Keyboard::D))
                   rect1.move(0.1f, 0.0f);
               else if (Keyboard::isKeyPressed(Keyboard::A))
                   rect1.move(-0.1f, 0.0f);
               else if (Keyboard::isKeyPressed(Keyboard::W))
                   rect1.move(0.0f, -0.1f);
               else if (Keyboard::isKeyPressed(Keyboard::S))
                   rect1.move(0.0f, 0.1f);
           }
           Packet packet;

           if (p1Position != rect1.getPosition())
           {
               packet << rect1.getPosition().x << rect1.getPosition().y;
               socket.send(packet);
           }
           socket.receive(packet);
           if (packet >> p2Position.x >> p2Position.y)
           {
               rect2.setPosition(p2Position);
           }

           window.clear();
           window.draw(rect1);
           window.draw(rect2);
           window.draw(rect3);
           window.display();
       }
       */
   /*
    //Network
    IpAddress ip = IpAddress::getLocalAddress();
    TcpSocket socket;
    TcpSocket socket2;
    TcpSocket socket3;
    char connectionType;
    char Buffer[2000];
    char Buffer2[2000];
    char Buffer3[2000];
    size_t received;
    size_t received2;
    size_t received3;

    cout << "Enter 'S' for Server or enter 'C' for Client: ";
    cin >> connectionType;

    if (connectionType == 's')
    {
        //SERVER
        TcpListener listener;
        listener.listen(2000);
        listener.accept(socket);
        listener.accept(socket2);
        listener.accept(socket3);
    }
    else if (connectionType == 'c')
    {
        //CLIENT
        socket.connect(ip, 2000);
        string text;
        getline(cin, text);
        while (1)
        {
            if (cin >> text)
            {
                socket.send(text.c_str(), text.length() + 1);
            }
        }
    }
    while (1)
    {
        socket.receive(Buffer, sizeof(Buffer), received);
        socket2.receive(Buffer2, sizeof(Buffer2), received);
        cout << Buffer << endl;
        cout << Buffer2 << endl;
        cout << Buffer3 << endl;
    }

    system("pause");
}*/