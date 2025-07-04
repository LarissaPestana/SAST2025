
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/wave-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor.h"
#include "ns3/ipv4-flow-classifier.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("VehicularComparisonSim");

FlowMonitorHelper flowHelper;

void Setup80211p ()
{
  std::cout << "\n=== INICIANDO SIMULAÇÃO: IEEE 802.11p ===\n" << std::endl;

  NodeContainer nodes;
  nodes.Create (2);

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (100.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  mobility.Install (nodes);
  nodes.Get (0)->GetObject<ConstantVelocityMobilityModel> ()->SetVelocity (Vector (10.0, 0.0, 0.0));
  nodes.Get (1)->GetObject<ConstantVelocityMobilityModel> ()->SetVelocity (Vector (10.0, 0.0, 0.0));

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy;
  phy.SetChannel (channel.Create ());
  NqosWaveMacHelper mac = NqosWaveMacHelper::Default ();
  Wifi80211pHelper wifi = Wifi80211pHelper::Default ();
  NetDeviceContainer devices = wifi.Install (phy, mac, nodes);

  InternetStackHelper internet;
  internet.Install (nodes);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = ipv4.Assign (devices);

  uint16_t port = 9;
  UdpEchoServerHelper echoServer (port);
  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (0.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), port);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (10));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (512));
  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (1.0));
  clientApps.Stop (Seconds (10.0));

  Ptr<FlowMonitor> flowMonitor = flowHelper.InstallAll ();
  Simulator::Stop (Seconds (11.0));
  Simulator::Run ();

  flowMonitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowHelper.GetClassifier ());
  std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats ();

  for (const auto& flow : stats)
  {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (flow.first);
    std::cout << "[802.11p] Flow " << flow.first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
    std::cout << "  Tx Packets: " << flow.second.txPackets << "\n";
    std::cout << "  Rx Packets: " << flow.second.rxPackets << "\n";
    std::cout << "  Lost Packets: " << flow.second.lostPackets << "\n";
    std::cout << "  Delay Sum: " << flow.second.delaySum.GetSeconds () << "s\n";
    std::cout << "  Jitter Sum: " << flow.second.jitterSum.GetSeconds () << "s\n";
    std::cout << "  Throughput: "
              << (flow.second.rxBytes * 8.0 /
                  (flow.second.timeLastRxPacket.GetSeconds () -
                   flow.second.timeFirstTxPacket.GetSeconds () + 1e-9)) /
                     1e6
              << " Mbps\n\n";
  }

  Simulator::Destroy ();
}

void SetupSimulated80211bd ()
{
  std::cout << "\n=== INICIANDO SIMULAÇÃO: IEEE 802.11bd (Simulado) ===\n" << std::endl;

  NodeContainer nodes;
  nodes.Create (2);

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 10.0, 0.0));
  positionAlloc->Add (Vector (100.0, 10.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  mobility.Install (nodes);
  nodes.Get (0)->GetObject<ConstantVelocityMobilityModel> ()->SetVelocity (Vector (10.0, 0.0, 0.0));
  nodes.Get (1)->GetObject<ConstantVelocityMobilityModel> ()->SetVelocity (Vector (10.0, 0.0, 0.0));

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy;
  phy.SetChannel (channel.Create ());
  phy.Set ("TxPowerStart", DoubleValue (23));
  phy.Set ("TxPowerEnd", DoubleValue (23));
  phy.Set ("RxGain", DoubleValue (5));

  NqosWaveMacHelper mac = NqosWaveMacHelper::Default ();
  Wifi80211pHelper wifi = Wifi80211pHelper::Default ();
  NetDeviceContainer devices = wifi.Install (phy, mac, nodes);

  InternetStackHelper internet;
  internet.Install (nodes);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = ipv4.Assign (devices);

  uint16_t port = 9;
  UdpEchoServerHelper echoServer (port);
  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (0.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), port);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (10));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (512));
  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (1.0));
  clientApps.Stop (Seconds (10.0));

  Ptr<FlowMonitor> flowMonitor = flowHelper.InstallAll ();
  Simulator::Stop (Seconds (11.0));
  Simulator::Run ();

  flowMonitor->CheckForLostPackets ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowHelper.GetClassifier ());
  std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats ();

  for (const auto& flow : stats)
  {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (flow.first);
    std::cout << "[802.11bd] Flow " << flow.first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
    std::cout << "  Tx Packets: " << flow.second.txPackets << "\n";
    std::cout << "  Rx Packets: " << flow.second.rxPackets << "\n";
    std::cout << "  Lost Packets: " << flow.second.lostPackets << "\n";
    std::cout << "  Delay Sum: " << flow.second.delaySum.GetSeconds () << "s\n";
    std::cout << "  Jitter Sum: " << flow.second.jitterSum.GetSeconds () << "s\n";
    std::cout << "  Throughput: "
              << (flow.second.rxBytes * 8.0 /
                  (flow.second.timeLastRxPacket.GetSeconds () -
                   flow.second.timeFirstTxPacket.GetSeconds () + 1e-9)) /
                     1e6
              << " Mbps\n\n";
  }

  Simulator::Destroy ();
}

int main (int argc, char *argv[])
{
  Setup80211p ();
  SetupSimulated80211bd ();
  return 0;
}
