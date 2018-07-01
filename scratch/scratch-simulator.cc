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
    //LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    //LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    /* Configuracao dos nos (hosts) */
    NodeContainer applA;    // Container para os servidores da Rede A
    NodeContainer wifiA;    // Container para os hosts da Rede wireless A
    NodeContainer subNetA1; // Containers para os hosts da Rede A
    NodeContainer subNetA2;
    NodeContainer subNetA3;
    NodeContainer subNetA4;
    NodeContainer subNetA5;
    NodeContainer switchA;  // Containers para os switches da Rede A

    NodeContainer applB;    // Container para os servidores da Rede B
    NodeContainer wifiB;    // Container para os hosts da Rede wireless B
    NodeContainer subNetB1; // Containers para os hosts da Rede B
    NodeContainer subNetB2;
    NodeContainer subNetB3;
    NodeContainer subNetB4;
    NodeContainer subNetB5;
    NodeContainer switchB;  // Containers para os switches da Rede B

    NodeContainer nodesWAN; // Container para os hosts da WAN

    applA.Create(3);    // Criacao dos hosts em si
    wifiA.Create(25);
    subNetA1.Create(25);
    subNetA2.Create(25);
    subNetA3.Create(25);
    subNetA4.Create(25);
    subNetA5.Create(25);
    switchA.Create(5);

    applB.Create(3);
    wifiB.Create(25);
    subNetB1.Create(25);
    subNetB2.Create(25);
    subNetB3.Create(25);
    subNetB4.Create(25);
    subNetB5.Create(25);
    switchB.Create(5);

    nodesWAN.Create(12);

    /* Configuracao dos enlaces */
    PointToPointHelper etherCat5;   // Criacao dos enlaces
    PointToPointHelper etherCat6;   // Utilizam cabos de par trancado (Ethernet, categorias 5e, 6 e 6A)
    PointToPointHelper etherCat6A;

    PointToPointHelper oc12;        // Utilizam fibra otica, padrao Optical Carrier (OC-12/48/192)
    PointToPointHelper oc48;
    PointToPointHelper oc192;

    etherCat5.SetDeviceAttribute("DataRate", StringValue("1Gbps")); // Configuracao dos atributos dos cabos ethernet
    etherCat5.SetChannelAttribute("Delay", StringValue("1ms"));

    etherCat6.SetDeviceAttribute("DataRate",
                                 StringValue("5Gbps")); // Configurados de acordo com as especificacoes tecnicas
    etherCat6.SetChannelAttribute("Delay",
                                  StringValue("1ms"));     // Configurados de acordo com o uso tipico dentro de uma LAN

    etherCat6A.SetDeviceAttribute("DataRate", StringValue("10Gbps"));
    etherCat6A.SetChannelAttribute("Delay", StringValue("1ms"));

    oc12.SetDeviceAttribute("DataRate",
                            StringValue("622Mbps"));    // Configuracao dos atributos dos cabos de fibra otica
    oc12.SetChannelAttribute("Delay", StringValue("5ms"));

    oc48.SetDeviceAttribute("DataRate",
                            StringValue("2488Mbps"));   // Configurados de acordo com as especificacoes tecnicas
    oc48.SetChannelAttribute("Delay",
                             StringValue("5ms"));         // Configurados de acordo com o uso tipico dentro de uma WAN

    oc192.SetDeviceAttribute("DataRate", StringValue("9953Mbps"));
    oc192.SetChannelAttribute("Delay", StringValue("5ms"));

    NetDeviceContainer subNetA, aux;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 25; j++) {
            if (i == 0) {
                aux = etherCat6A.Install(subNetA1.Get(j), switchA.Get(i));
                subNetA.Add(aux.Get(0));
            }
            else if (i == 1) {
                aux = etherCat6A.Install(subNetA2.Get(j), switchA.Get(i));
                subNetA.Add(aux.Get(0));
            }
            else if (i == 2) {
                aux = etherCat6A.Install(subNetA3.Get(j), switchA.Get(i));
                subNetA.Add(aux.Get(0));
            }
            else if (i == 3) {
                aux = etherCat6A.Install(subNetA4.Get(j), switchA.Get(i));
                subNetA.Add(aux.Get(0));
            }
            else {
                aux = etherCat6A.Install(subNetA5.Get(j), switchA.Get(i));
                subNetA.Add(aux.Get(0));
            }
        }
    }
    Ipv4AddressHelper address;
    Ipv4InterfaceContainer interfaces;
    address.SetBase("193.1.17.32", "255.255.255.0");
    interfaces = address.Assign(subNetA);

    NetDeviceContainer subNetB;
    for (int i = 0; i < 5; i ++) {
        for (int j = 0; j < 25; j++) {
            if (i == 0) {
                aux = etherCat5.Install(subNetB1.Get(j), switchA.Get(i));
                subNetB.Add(aux.Get(0));
            }
            else if (i == 1) {
                aux = etherCat5.Install(subNetB2.Get(j), switchA.Get(i));
                subNetB.Add(aux.Get(0));
            }
            else if (i == 2) {
                aux = etherCat5.Install(subNetB3.Get(j), switchA.Get(i));
                subNetB.Add(aux.Get(0));
            }
            else if (i == 3) {
                aux = etherCat5.Install(subNetB4.Get(j), switchA.Get(i));
                subNetB.Add(aux.Get(0));
            }
            else {
                aux = etherCat5.Install(subNetB5.Get(j), switchA.Get(i));
                subNetB.Add(aux.Get(0));
            }
        }
    }
    address.SetBase("193.1.28.32", "255.255.255.0");
    interfaces = address.Assign(subNetB);





    Simulator::Run();       // Inicio da simulacao
    Simulator::Destroy();   // Fim da simulacao

    return 0;
}