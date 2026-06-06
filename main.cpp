#include "C:/SFML_VS/SFML-2.6.1/include/SFML/Graphics.hpp"
#include <vector>
#include <cstdlib> 
#include <ctime>   
#include <string>  

#pragma comment(lib, "C:/SFML_VS/SFML-2.6.1/lib/sfml-graphics.lib")
#pragma comment(lib, "C:/SFML_VS/SFML-2.6.1/lib/sfml-window.lib")
#pragma comment(lib, "C:/SFML_VS/SFML-2.6.1/lib/sfml-system.lib")

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main() {
    std::srand(static_cast<unsigned>(std::time(0)));

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Flappy Bird Clone");
    window.setFramerateLimit(60);

    // --- GÖRSELLERİ (TEXTURE) YÜKLEME ---
    sf::Texture bgTex, birdTex, pipeTex;
    // Dosyaların font ile aynı klasörde (.exe veya main.cpp'nin yanında) olması şart!
    bgTex.loadFromFile("background.png");
    birdTex.loadFromFile("bird.png");
    pipeTex.loadFromFile("pipe.png");

    // Arka plan Sprite'ı
    sf::Sprite background(bgTex);
    // Eğer arka plan küçükse tam ekrana sığdır:
    // background.setScale(WINDOW_WIDTH / background.getLocalBounds().width, WINDOW_HEIGHT / background.getLocalBounds().height);

    // Kuş Sprite'ı
    sf::Sprite bird(birdTex);
    bird.setPosition(100.0f, WINDOW_HEIGHT / 2.0f);
    // Kuşun merkez noktasını ayarla (Rotasyon ve fizik için daha sağlıklı olur)
    sf::FloatRect birdBounds = bird.getLocalBounds();
    bird.setOrigin(birdBounds.width / 2.0f, birdBounds.height / 2.0f);

    // --- KULLANICI ARAYÜZÜ ---
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20.0f, 20.0f);
    // Yazının etrafına siyah çizgi ekleyelim ki arka plana karışmasın
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2.0f);

    int score = 0;
    int deletedPipes = 0;

    // Fizik
    float gravity = 0.6f;
    float velocity = 0.0f;
    float flapStrength = -10.0f;

    // Boru Değişkenleri
    std::vector<sf::Sprite> pipes;
    float pipeSpeed = 4.0f;
    int spawnTimer = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    velocity = flapStrength;
                }
            }
        }

        // --- FİZİK GÜNCELLEMESİ ---
        velocity += gravity;
        bird.move(0, velocity);

        // Kuş düşerken burnu aşağı, zıplarken yukarı baksın (Küçük bir animasyon detayı)
        bird.setRotation(velocity * 3.0f);

        // Boru Üretme
      // Boru Üretme
        spawnTimer++;
        if (spawnTimer >= 100) {
            spawnTimer = 0;
            float gapPosition = (std::rand() % 300) + 100.0f;
            float gapSize = 150.0f;

            // Borunun boyu ekrana KESİN yetsin diye pencere yüksekliğine oranlıyoruz
            float scaleY = WINDOW_HEIGHT / (float)pipeTex.getSize().y;
            float scaleX = 1.5f; // Borular biraz daha tok/kalın dursun diye X'i de büyütüyoruz

            // Üst Boru
            sf::Sprite topPipe(pipeTex);
            topPipe.setScale(scaleX, -scaleY); // Y'yi eksi yapıyoruz ki baş aşağı dönsün
            topPipe.setPosition(WINDOW_WIDTH, gapPosition);
            pipes.push_back(topPipe);

            // Alt Boru
            sf::Sprite bottomPipe(pipeTex);
            bottomPipe.setScale(scaleX, scaleY);
            bottomPipe.setPosition(WINDOW_WIDTH, gapPosition + gapSize);
            pipes.push_back(bottomPipe);
        }
        // Çarpışma ve Hareket
        // Çarpışma ve Hareket
        for (size_t i = 0; i < pipes.size(); i++) {
            pipes[i].move(-pipeSpeed, 0);

            // --- HASSAS ÇARPIŞMA (Merkezcil Hitbox Daraltma) ---
            sf::FloatRect birdHitbox = bird.getGlobalBounds();

            float shrinkFactor = 0.6f; // Kutuyu orijinal boyutunun %60'ına düşürüyoruz (çok azıcık daha küçülttük)

            // Kutuyu sadece küçültmekle kalmıyor, tam ortaya hizalıyoruz
            float xOffset = (birdHitbox.width - (birdHitbox.width * shrinkFactor)) / 2.0f;
            float yOffset = (birdHitbox.height - (birdHitbox.height * shrinkFactor)) / 2.0f;

            birdHitbox.left += xOffset;
            birdHitbox.top += yOffset;
            birdHitbox.width *= shrinkFactor;
            birdHitbox.height *= shrinkFactor;

            // Küçültülmüş ve ortalanmış kuş kutusu, borunun tam kutusuyla kesişiyor mu?
            if (birdHitbox.intersects(pipes[i].getGlobalBounds())) {
                pipes.clear();
                bird.setPosition(100.0f, WINDOW_HEIGHT / 2.0f);
                velocity = 0;
                score = 0;
                deletedPipes = 0;
                bird.setRotation(0);
                break;
            }
        }

        // Skor Artışı ve Hafızadan Silme
        if (!pipes.empty() && pipes.front().getPosition().x < -pipes.front().getGlobalBounds().width) {
            pipes.erase(pipes.begin());
            deletedPipes++;
            score = deletedPipes / 2;
        }

        // Ekran Dışı Kontrolü
        if (bird.getPosition().y > WINDOW_HEIGHT || bird.getPosition().y < 0) {
            pipes.clear();
            bird.setPosition(100.0f, WINDOW_HEIGHT / 2.0f);
            velocity = 0;
            score = 0;
            deletedPipes = 0;
            bird.setRotation(0);
        }
        // Arka plan Sprite'ı
        sf::Sprite background(bgTex);

        // Arka planı 800x600 oyun penceresine tam sığacak şekilde esnetiyoruz:
        background.setScale(
            WINDOW_WIDTH / background.getLocalBounds().width,
            WINDOW_HEIGHT / background.getLocalBounds().height
        );
        scoreText.setString("Skor: " + std::to_string(score));

        // --- ÇİZİM ---
        window.clear();

        window.draw(background); // Önce arkaplanı çiz (en arkada kalır)
        for (auto& pipe : pipes) {
            window.draw(pipe);   // Sonra boruları çiz
        }
        window.draw(bird);       // Kuşu en öne çiz
        window.draw(scoreText);  // Skoru en üste yazdır

        window.display();
    }

    return 0;
}