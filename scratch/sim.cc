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
    /* Logs utilizados */
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    /* Configuracao dos nos (hosts) */
    NodeContainer nodesA;   // Container para os hosts da Rede A
    NodeContainer nodesB;   // Container para os hosts da Rede B
    NodeContainer nodesLAN; // Container para os hosts da LAN

    nodesA.Create(161);     // Criacao dos hosts em si
    nodesB.Create(161);
    nodesLAN.Create(10);

    /* Configuracao dos enlaces */
    PointToPointHelper etherCat5;   // Criacao dos enlaces
    PointToPointHelper etherCat6;   // Utilizam cabos de par trancado (Ethernet, categorias 5e, 6 e 6A)
    PointToPointHelper etherCat6A;

    PointToPointHelper oc12;        // Utilizam fibra otica, padrao Optical Carrier (OC-12/48/192)
    PointToPointHelper oc48;
    PointToPointHelper oc192;

    etherCat5.SetDeviceAttribute("DataRate", StringValue("1Gbps")); // Configuracao dos atributos dos cabos ethernet
    etherCat5.SetChannelAttribute("Delay", StringValue("1ms"));

    etherCat6.SetDeviceAttribute("DataRate", StringValue("5Gbps")); // Configurados de acordo com as especificacoes tecnicas
    etherCat6.SetChannelAttribute("Delay", StringValue("1ms"));     // Configurados de acordo com o uso tipico dentro de uma LAN

    etherCat6A.SetDeviceAttribute("DataRate", StringValue("10Gbps"));
    etherCat6A.SetChannelAttribute("Delay", StringValue("1ms"));

    oc12.SetDeviceAttribute("DataRate", StringValue("622Mbps"));    // Configuracao dos atributos dos cabos de fibra otica
    oc12.SetChannelAttribute("Delay", StringValue("5ms"));

    oc48.SetDeviceAttribute("DataRate", StringValue("2488Mbps"));   // Configurados de acordo com as especificacoes tecnicas
    oc48.SetChannelAttribute("Delay", StringValue("5ms"));         // Configurados de acordo com o uso tipico dentro de uma WAN

    oc192.SetDeviceAttribute("DataRate", StringValue("9953Mbps"));
    oc192.SetChannelAttribute("Delay", StringValue("5ms"));

    Simulator::Run();       // Inicio da simulacao
    Simulator::Destroy();   // Fim da simulacao

    return 0;
}