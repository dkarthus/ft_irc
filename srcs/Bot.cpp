//
// Created by Kingler Alexandria on 5/23/22.
//

#include "Server.hpp"
#include <iostream>
#include <string>
#include <vector>


std::string answerBot()
{
    std::vector<std::string> answers;

    answers.push_back("Насколько я вижу да");
    answers.push_back("Спроси позже");
    answers.push_back("Не могу сейчас сказать");
    answers.push_back("Соберись с мыслями и спроси снова");
    answers.push_back("Не рассчитывай на это");
    answers.push_back("Я так не думаю");
    answers.push_back("Я сомневаюсь насчёт этого");
    answers.push_back("Это бесспорно");
    answers.push_back("Да это так");
    answers.push_back("Может быть");
    answers.push_back("Наиболее вероятно");
    answers.push_back("Мои источники говорят нет");
    answers.push_back("Мои источники говорят да");
    answers.push_back("НЕТ!");
    answers.push_back("Перспектива хорошая");
    answers.push_back("Перспектива не очень хорошая");
    answers.push_back("Будущее туманно спроси позже");
    answers.push_back("Знаки указывают что да");
    answers.push_back("Знаки указывают что нет");
    answers.push_back("Извини, нет");
    answers.push_back("Очень сомневаюсь");
    answers.push_back("Без сомнения");
    answers.push_back("ДА!");
    answers.push_back("Определённо да");
    answers.push_back("Ты можешь надеяться на это");

    return answers[rand() % 24];

}

void Server::sendPrivmsgToBot(int fd, std::string mes, std::string sender,User *user)
{

    std::string answer = answerBot();
    std::string mesfrombot =  ":bot!Adium@127.0.0.1 PRIVMSG " + sender + " :" +answer+"\n";

    size_t pos = mes.find("ирку");
    size_t pos2 = mes.find("ирка");
    size_t pos3 = mes.find("проект");
    if (pos != std::string::npos || pos2 != std::string::npos ||pos3 != std::string::npos)
        std::string mesfrombot =  ":bot!Adium@127.0.0.1 PRIVMSG " + sender + " :" +"Определённо да"+"\n";

    send(fd, mesfrombot.c_str(), mesfrombot.size(), 0);
}

