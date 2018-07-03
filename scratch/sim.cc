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

    /* -------------------------------------------------------------------------------------------------------------- */
    /* Configuracao dos hosts */
    NS_LOG_INFO("Inicializacao dos hosts.");
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

    NodeContainer routers; // Container para os roteadores

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

    routers.Create(12);


    // <@ghalestrilo>
    std::vector<NodeContainer*> subnetsLANA;
    subnetsLANA.push_back(&subNetA1); // Containers para os hosts da Rede A
    subnetsLANA.push_back(&subNetA2);
    subnetsLANA.push_back(&subNetA3);
    subnetsLANA.push_back(&subNetA4);
    subnetsLANA.push_back(&subNetA5);
    
    std::vector<NodeContainer*> subnetsLANB;
    subnetsLANB.push_back(&subNetB1); // Containers para os hosts da Rede A
    subnetsLANB.push_back(&subNetB2);
    subnetsLANB.push_back(&subNetB3);
    subnetsLANB.push_back(&subNetB4);
    subnetsLANB.push_back(&subNetB5);
    
    // </@ghalestrilo>


    /* -------------------------------------------------------------------------------------------------------------- */
    /* Instalacao da da internet stack */
    NS_LOG_INFO("Instalacao da internet stack");
    InternetStackHelper stack;

    stack.Install(applA);       // Dispositivos da Rede A
    stack.Install(wifiA);
    for (auto n : subnetsLANA)
        stack.Install(*n);
    stack.Install(switchA);

    stack.Install(applB);       // Dispositivos da Rede B
    stack.Install(wifiB);
    for (auto n : subnetsLANB)
        stack.Install(*n);
    stack.Install(switchB);

    stack.Install(routers);     // Roteadores

    /* -------------------------------------------------------------------------------------------------------------- */
    /* Configuracao dos enlaces */
    NS_LOG_INFO("Configuracao dos enlaces.");
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

    /* -------------------------------------------------------------------------------------------------------------- */
    /* Criacao dos containers para os dispositivos de rede e configuracao do enderecamento  */
    NS_LOG_INFO("Processo de cabeamento dos roteadores.");
    NetDeviceContainer wanRouters;  // Container para os roteadores da WAN e os de borda das redes A e B

    wanRouters.Add(oc12.Install(routers.Get(0), routers.Get(1)));   // Roteador de borda Rede A -> Roteador de nucleo 1
    wanRouters.Add(oc12.Install(routers.Get(9), routers.Get(11)));  // Roteador de borda Rede B -> Roteador de nucleo 11

    for (int i = 1; i < 5; i++) {   // Laco para "cabeamento" dos roteadores seguindo os seguintes parametros:
        wanRouters.Add(oc192.Install(routers.Get(2*i - 1), routers.Get(2*i + 1)));  // Impar -> impar;
        wanRouters.Add(oc192.Install(routers.Get(i*2), routers.Get( (i+1)*2 )));    // Par -> par.
    }

    wanRouters.Add(oc48.Install(routers.Get(2), routers.Get(3)));   // Cabeamento do roteador 2 -> 3
    wanRouters.Add(oc192.Install(routers.Get(3), routers.Get(4)));  // Cabeamento do roteador 3 -> 4
    wanRouters.Add(oc192.Install(routers.Get(5), routers.Get(6)));  // Cabeamento do roteador 5 -> 6
    wanRouters.Add(oc192.Install(routers.Get(7), routers.Get(8)));  // Cabeamento do roteador 7 -> 8
    wanRouters.Add(oc48.Install(routers.Get(7), routers.Get(10)));  // Cabeamento do roteador 7 -> 10

    NS_LOG_INFO("Processo enderecamento dos roteadores de borda e da WAN.");
    Ipv4AddressHelper address;                          // Criacao de um auxiliador de enderecamento
    Ipv4InterfaceContainer interfaces;                  // Criacao do container de interfaces IPv4

    address.SetBase("193.1.17.0", "255.255.248.0");   // Endereco base e mascara de sub-rede dos roteadores (WAN e borda)
    interfaces = address.Assign(wanRouters);        // Processo de enderecamento dos roteadores (WAN e borda)



    // Enderecamento LAN A

    NS_LOG_INFO("Laco de cabeamento dos dispositivos da Rede A.");
    NetDeviceContainer subNetA;    // Container para a Rede A

    // Laco de cabeamento hosts -> switches -> roteador da Rede A
    for(int i = 0; i < subnetsLANA.size(); i++){
        
        // "Cabeamento" hosts -> switch em cada subrede
        for(int j = 0; j < 25; j++)
            subNetA.Add((etherCat6A.Install(subnetsLANA[i]->Get(j), switchA.Get(i))).Get(0));
        
        // "Cabeamento" switches -> roteador
        subNetA.Add(etherCat6A.Install(switchA.Get(i), routers.Get(0)));
    }
    NS_LOG_INFO("Enderecamento da Rede A.");
    address.SetBase("193.1.17.32", "255.255.255.0");    // Endereco base e mascara de sub-rede da Rede A
    interfaces = address.Assign(subNetA);               // Processo de enderecamento da Rede A

    
    
    
    // Enderecamento LAN B
    
    NS_LOG_INFO("Laco de cabeamento da Rede B.");
    NetDeviceContainer subNetB;         // Container para os dispositivos da Rede B
    
    // Laco de cabeamento hosts -> switches -> roteador da Rede A
    for(int i = 0; i < subnetsLANB.size(); i++){
        
        // "Cabeamento" hosts -> switch em cada subrede
        for(int j = 0; j < 25; j++)
            subNetB.Add((etherCat5.Install(subnetsLANB[i]->Get(j), switchB.Get(i))).Get(0));
        
        // "Cabeamento" switches -> roteador
        subNetB.Add(etherCat6.Install(switchB.Get(i), routers.Get(11))); // 11?
    }
    
    NS_LOG_INFO("Enderecamento da Rede B.");
    address.SetBase("193.1.28.32", "255.255.255.0");    // Definicao do endereco base e mascara de sub-rede da Rede B
    interfaces = address.Assign(subNetB);               // Processo de enderecamento da Rede B



    Simulator::Run();       // Inicio da simulacao
    Simulator::Destroy();   // Fim da simulacao

    return 0;
}
