/*
 * Simulador da rede para o Laboratorio 4 da disciplina Redes de Computadores
 * Codigo criado em 30/06/18;
 * Autores: Pedro Torres, Thales Grilo e Icaro Rezende.
*/

// Modulos utilizados
#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/applications-module.h>
#include <ns3/simulator.h>

// Declaracao de namespace
using namespace ns3;

NS_LOG_COMPONENT_DEFINE("RedeLab4");    // Nome do log que sera gerado

int main() {
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO); //
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    NodeContainer nodesA;   // Container para os hosts (nos) da Rede A
    NodeContainer nodesB;   // Container para os hosts (nos) da Rede B
    NodeContainer nodesLAN; // Container para os hosts (nos) da LAN

    nodesA.Create(161);     // Criacao dos hosts em si
    nodesB.Create(161);
    nodesLAN.Create(10);

    Simulator::Run();       // Inicio da simulacao
    Simulator::Destroy();   // Fim da simulacao

    return 0;
}