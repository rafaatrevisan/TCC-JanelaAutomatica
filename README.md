# OpenWindow - Janela Automatizada

O projeto OpenWindow consiste em um sistema para automatizar janelas, fazendo com que a mesma feche quando detectada a presença de chuva e assim proporcionando mais segurança. O sistema utiliza os componentes:

- ESP8266
- Sensor de chuva
- Chave fim de curso
- Motor de passo

O principal diferencial do projeto é a integração à um chatbot no Telegram, que permite controle da janela remotamente, proporcionando maior controle e comodidade, podendo de qualquer lugar:

- Abrir a janela
- Fechar a janela
- Verificar o status (se está aberta ou fechada)

O repositório contém o arquivo .ino da programação. Para utilizá-lo, você ainda precisará dos componentes citados acima. Lembre de mudar as variáveis de porta dos componentes (caso necessário), variáveis de conexão com o WIFI e conexão com bot do Telegram. Se desejar utilizar o próprio bot que já está incluído na programação ao invés de criar um novo, pesquise por "@OpenWindowTeste_bot" no Telegram e utilize-o.