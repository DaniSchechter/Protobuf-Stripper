#include "server.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "IsEncryptedRawData.hpp"
#include <string>
#include <boost/bind.hpp>
#include "PortsTest.hpp"

int main(int argc, char* argv[])
{
  // std::cout << Entropy("Saw yet kindness too replying whatever marianne. Old sentiments resolution admiration unaffected its mrs literature. Behaviour new set existence dashwoods. It satisfied to mr commanded consisted disposing engrossed. Tall snug do of till on easy. Form not calm new fail. Ten the hastened steepest feelings pleasant few surprise property. An brother he do colonel against minutes uncivil. Can how elinor warmly mrs basket marked. Led raising expense yet demesne weather musical. Me mr what park next busy ever. Elinor her his secure far twenty eat object. Late any far saw size want man. Which way you wrong add shall one. As guest right of he scale these. Horses nearer oh elinor of denote. Certainty listening no no behaviour existence assurance situation is. Because add why not esteems amiable him. Interested the unaffected mrs law friendship add principles. Indeed on people do merits to. Court heard which up above hoped grave do. Answer living law things either sir bed length. Looked before we an on merely. These no death he at share alone. Yet outward the him compass hearted are tedious. At as in understood an remarkably solicitude. Mean them very seen she she. Use totally written the observe pressed justice. Instantly cordially far intention recommend estimable yet her his. Ladies stairs enough esteem add fat all enable. Needed its design number winter see. Oh be me sure wise sons no. Piqued ye of am spirit regret. Stimulated discretion impossible admiration in particular conviction up. His exquisite sincerity education shameless ten earnestly breakfast add. So we me unknown as improve hastily sitting forming. Especially favourable compliment but thoroughly unreserved saw she themselves. Sufficient impossible him may ten insensible put continuing. Oppose exeter income simple few joy cousin but twenty. Scale began quiet up short wrong in in. Sportsmen shy forfeited engrossed may can. Sigh view am high neat half to what. Sent late held than set why wife our. If an blessing building steepest. Agreement distrusts mrs six affection satisfied. Day blushes visitor end company old prevent chapter. Consider declared out expenses her concerns. No at indulgence conviction particular unsatiable boisterous discretion. Direct enough off others say eldest may exeter she. Possible all ignorant supplied get settling marriage recurred. Your it to gave life whom as. Favourable dissimilar resolution led for and had. At play much to time four many. Moonlight of situation so if necessary therefore attending abilities. Calling looking enquire up me to in removal. Park fat she nor does play deal our. Procured sex material his offering humanity laughing moderate can. Unreserved had she nay dissimilar admiration interested. Departure performed exquisite rapturous so ye me resources. An so vulgar to on points wanted. Not rapturous resolving continued household northward gay. He it otherwise supported instantly. Unfeeling agreeable suffering it on smallness newspaper be. So come must time no as. Do on unpleasing possession as of unreserved. Yet joy exquisite put sometimes enjoyment perpetual now. Behind lovers eat having length horses vanity say had its. Am of mr friendly by strongly peculiar juvenile. Unpleasant it sufficient simplicity am by friendship no inhabiting. Goodness doubtful material has denoting suitable she two. Dear mean she way and poor bred they come. He otherwise me incommode explained so in remaining. Polite barton in it warmly do county length an. Mr do raising article general norland my hastily. Its companions say uncommonly pianoforte favourable. Education affection consulted by mr attending he therefore on forfeited. High way more far feet kind evil play led. Sometimes furnished collected add for resources attention. Norland an by minuter enquire it general on towards forming. Adapted mrs totally company two yet conduct men.") << '\n';
  // std::cout << Entropy("F5789E5AAA87C834CD38C4CA6205A66C") << '\n';
  // std::cout << Entropy("U2F3IHlldCBraW5kbmVzcyB0b28gcmVwbHlpbmcgd2hhdGV2ZXIgbWFyaWFubmUuIE9sZCBzZW50aW1lbnRzIHJlc29sdXRpb24gYWRtaXJhdGlvbiB1bmFmZmVjdGVkIGl0cyBtcnMgbGl0ZXJhdHVyZS4gQmVoYXZpb3VyIG5ldyBzZXQgZXhpc3RlbmNlIGRhc2h3b29kcy4gSXQgc2F0aXNmaWVkIHRvIG1yIGNvbW1hbmRlZCBjb25zaXN0ZWQgZGlzcG9zaW5nIGVuZ3Jvc3NlZC4gVGFsbCBzbnVnIGRvIG9mIHRpbGwgb24gZWFzeS4gRm9ybSBub3QgY2FsbSBuZXcgZmFpbC4gVGVuIHRoZSBoYXN0ZW5lZCBzdGVlcGVzdCBmZWVsaW5ncyBwbGVhc2FudCBmZXcgc3VycHJpc2UgcHJvcGVydHkuIEFuIGJyb3RoZXIgaGUgZG8gY29sb25lbCBhZ2FpbnN0IG1pbnV0ZXMgdW5jaXZpbC4gQ2FuIGhvdyBlbGlub3Igd2FybWx5IG1ycyBiYXNrZXQgbWFya2VkLiBMZWQgcmFpc2luZyBleHBlbnNlIHlldCBkZW1lc25lIHdlYXRoZXIgbXVzaWNhbC4gTWUgbXIgd2hhdCBwYXJrIG5leHQgYnVzeSBldmVyLiBFbGlub3IgaGVyIGhpcyBzZWN1cmUgZmFyIHR3ZW50eSBlYXQgb2JqZWN0LiBMYXRlIGFueSBmYXIgc2F3IHNpemUgd2FudCBtYW4uIFdoaWNoIHdheSB5b3Ugd3JvbmcgYWRkIHNoYWxsIG9uZS4gQXMgZ3Vlc3QgcmlnaHQgb2YgaGUgc2NhbGUgdGhlc2UuIEhvcnNlcyBuZWFyZXIgb2ggZWxpbm9yIG9mIGRlbm90ZS4gQ2VydGFpbnR5IGxpc3RlbmluZyBubyBubyBiZWhhdmlvdXIgZXhpc3RlbmNlIGFzc3VyYW5jZSBzaXR1YXRpb24gaXMuIEJlY2F1c2UgYWRkIHdoeSBub3QgZXN0ZWVtcyBhbWlhYmxlIGhpbS4gSW50ZXJlc3RlZCB0aGUgdW5hZmZlY3RlZCBtcnMgbGF3IGZyaWVuZHNoaXAgYWRkIHByaW5jaXBsZXMuIEluZGVlZCBvbiBwZW9wbGUgZG8gbWVyaXRzIHRvLiBDb3VydCBoZWFyZCB3aGljaCB1cCBhYm92ZSBob3BlZCBncmF2ZSBkby4gQW5zd2VyIGxpdmluZyBsYXcgdGhpbmdzIGVpdGhlciBzaXIgYmVkIGxlbmd0aC4gTG9va2VkIGJlZm9yZSB3ZSBhbiBvbiBtZXJlbHkuIFRoZXNlIG5vIGRlYXRoIGhlIGF0IHNoYXJlIGFsb25lLiBZZXQgb3V0d2FyZCB0aGUgaGltIGNvbXBhc3MgaGVhcnRlZCBhcmUgdGVkaW91cy4gQXQgYXMgaW4gdW5kZXJzdG9vZCBhbiByZW1hcmthYmx5IHNvbGljaXR1ZGUuIE1lYW4gdGhlbSB2ZXJ5IHNlZW4gc2hlIHNoZS4gVXNlIHRvdGFsbHkgd3JpdHRlbiB0aGUgb2JzZXJ2ZSBwcmVzc2VkIGp1c3RpY2UuIEluc3RhbnRseSBjb3JkaWFsbHkgZmFyIGludGVudGlvbiByZWNvbW1lbmQgZXN0aW1hYmxlIHlldCBoZXIgaGlzLiBMYWRpZXMgc3RhaXJzIGVub3VnaCBlc3RlZW0gYWRkIGZhdCBhbGwgZW5hYmxlLiBOZWVkZWQgaXRzIGRlc2lnbiBudW1iZXIgd2ludGVyIHNlZS4gT2ggYmUgbWUgc3VyZSB3aXNlIHNvbnMgbm8uIFBpcXVlZCB5ZSBvZiBhbSBzcGlyaXQgcmVncmV0LiBTdGltdWxhdGVkIGRpc2NyZXRpb24gaW1wb3NzaWJsZSBhZG1pcmF0aW9uIGluIHBhcnRpY3VsYXIgY29udmljdGlvbiB1cC4gSGlzIGV4cXVpc2l0ZSBzaW5jZXJpdHkgZWR1Y2F0aW9uIHNoYW1lbGVzcyB0ZW4gZWFybmVzdGx5IGJyZWFrZmFzdCBhZGQuIFNvIHdlIG1lIHVua25vd24gYXMgaW1wcm92ZSBoYXN0aWx5IHNpdHRpbmcgZm9ybWluZy4gRXNwZWNpYWxseSBmYXZvdXJhYmxlIGNvbXBsaW1lbnQgYnV0IHRob3JvdWdobHkgdW5yZXNlcnZlZCBzYXcgc2hlIHRoZW1zZWx2ZXMuIFN1ZmZpY2llbnQgaW1wb3NzaWJsZSBoaW0gbWF5IHRlbiBpbnNlbnNpYmxlIHB1dCBjb250aW51aW5nLiBPcHBvc2UgZXhldGVyIGluY29tZSBzaW1wbGUgZmV3IGpveSBjb3VzaW4gYnV0IHR3ZW50eS4gU2NhbGUgYmVnYW4gcXVpZXQgdXAgc2hvcnQgd3JvbmcgaW4gaW4uIFNwb3J0c21lbiBzaHkgZm9yZmVpdGVkIGVuZ3Jvc3NlZCBtYXkgY2FuLiBTaWdoIHZpZXcgYW0gaGlnaCBuZWF0IGhhbGYgdG8gd2hhdC4gU2VudCBsYXRlIGhlbGQgdGhhbiBzZXQgd2h5IHdpZmUgb3VyLiBJZiBhbiBibGVzc2luZyBidWlsZGluZyBzdGVlcGVzdC4gQWdyZWVtZW50IGRpc3RydXN0cyBtcnMgc2l4IGFmZmVjdGlvbiBzYXRpc2ZpZWQuIERheSBibHVzaGVzIHZpc2l0b3IgZW5kIGNvbXBhbnkgb2xkIHByZXZlbnQgY2hhcHRlci4gQ29uc2lkZXIgZGVjbGFyZWQgb3V0IGV4cGVuc2VzIGhlciBjb25jZXJucy4gTm8gYXQgaW5kdWxnZW5jZSBjb252aWN0aW9uIHBhcnRpY3VsYXIgdW5zYXRpYWJsZSBib2lzdGVyb3VzIGRpc2NyZXRpb24uIERpcmVjdCBlbm91Z2ggb2ZmIG90aGVycyBzYXkgZWxkZXN0IG1heSBleGV0ZXIgc2hlLiBQb3NzaWJsZSBhbGwgaWdub3JhbnQgc3VwcGxpZWQgZ2V0IHNldHRsaW5nIG1hcnJpYWdlIHJlY3VycmVkLiBZb3VyIGl0IHRvIGdhdmUgbGlmZSB3aG9tIGFzLiBGYXZvdXJhYmxlIGRpc3NpbWlsYXIgcmVzb2x1dGlvbiBsZWQgZm9yIGFuZCBoYWQuIEF0IHBsYXkgbXVjaCB0byB0aW1lIGZvdXIgbWFueS4gTW9vbmxpZ2h0IG9mIHNpdHVhdGlvbiBzbyBpZiBuZWNlc3NhcnkgdGhlcmVmb3JlIGF0dGVuZGluZyBhYmlsaXRpZXMuIENhbGxpbmcgbG9va2luZyBlbnF1aXJlIHVwIG1lIHRvIGluIHJlbW92YWwuIFBhcmsgZmF0IHNoZSBub3IgZG9lcyBwbGF5IGRlYWwgb3VyLiBQcm9jdXJlZCBzZXggbWF0ZXJpYWwgaGlzIG9mZmVyaW5nIGh1bWFuaXR5IGxhdWdoaW5nIG1vZGVyYXRlIGNhbi4gVW5yZXNlcnZlZCBoYWQgc2hlIG5heSBkaXNzaW1pbGFyIGFkbWlyYXRpb24gaW50ZXJlc3RlZC4gRGVwYXJ0dXJlIHBlcmZvcm1lZCBleHF1aXNpdGUgcmFwdHVyb3VzIHNvIHllIG1lIHJlc291cmNlcy4gQW4gc28gdnVsZ2FyIHRvIG9uIHBvaW50cyB3YW50ZWQuIE5vdCByYXB0dXJvdXMgcmVzb2x2aW5nIGNvbnRpbnVlZCBob3VzZWhvbGQgbm9ydGh3YXJkIGdheS4gSGUgaXQgb3RoZXJ3aXNlIHN1cHBvcnRlZCBpbnN0YW50bHkuIFVuZmVlbGluZyBhZ3JlZWFibGUgc3VmZmVyaW5nIGl0IG9uIHNtYWxsbmVzcyBuZXdzcGFwZXIgYmUuIFNvIGNvbWUgbXVzdCB0aW1lIG5vIGFzLiBEbyBvbiB1bnBsZWFzaW5nIHBvc3Nlc3Npb24gYXMgb2YgdW5yZXNlcnZlZC4gWWV0IGpveSBleHF1aXNpdGUgcHV0IHNvbWV0aW1lcyBlbmpveW1lbnQgcGVycGV0dWFsIG5vdy4gQmVoaW5kIGxvdmVycyBlYXQgaGF2aW5nIGxlbmd0aCBob3JzZXMgdmFuaXR5IHNheSBoYWQgaXRzLiBBbSBvZiBtciBmcmllbmRseSBieSBzdHJvbmdseSBwZWN1bGlhciBqdXZlbmlsZS4gVW5wbGVhc2FudCBpdCBzdWZmaWNpZW50IHNpbXBsaWNpdHkgYW0gYnkgZnJpZW5kc2hpcCBubyBpbmhhYml0aW5nLiBHb29kbmVzcyBkb3VidGZ1bCBtYXRlcmlhbCBoYXMgZGVub3Rpbmcgc3VpdGFibGUgc2hlIHR3by4gRGVhciBtZWFuIHNoZSB3YXkgYW5kIHBvb3IgYnJlZCB0aGV5IGNvbWUuIEhlIG90aGVyd2lzZSBtZSBpbmNvbW1vZGUgZXhwbGFpbmVkIHNvIGluIHJlbWFpbmluZy4gUG9saXRlIGJhcnRvbiBpbiBpdCB3YXJtbHkgZG8gY291bnR5IGxlbmd0aCBhbi4gTXIgZG8gcmFpc2luZyBhcnRpY2xlIGdlbmVyYWwgbm9ybGFuZCBteSBoYXN0aWx5LiBJdHMgY29tcGFuaW9ucyBzYXkgdW5jb21tb25seSBwaWFub2ZvcnRlIGZhdm91cmFibGUuIEVkdWNhdGlvbiBhZmZlY3Rpb24gY29uc3VsdGVkIGJ5IG1yIGF0dGVuZGluZyBoZSB0aGVyZWZvcmUgb24gZm9yZmVpdGVkLiBIaWdoIHdheSBtb3JlIGZhciBmZWV0IGtpbmQgZXZpbCBwbGF5IGxlZC4gU29tZXRpbWVzIGZ1cm5pc2hlZCBjb2xsZWN0ZWQgYWRkIGZvciByZXNvdXJjZXMgYXR0ZW50aW9uLiBOb3JsYW5kIGFuIGJ5IG1pbnV0ZXIgZW5xdWlyZSBpdCBnZW5lcmFsIG9uIHRvd2FyZHMgZm9ybWluZy4gQWRhcHRlZCBtcnMgdG90YWxseSBjb21wYW55IHR3byB5ZXQgY29uZHVjdCBtZW4u") << '\n';
  // std::cout << Entropy("BiGWxmvgvURyp2T4ruqnIdMVZhfeGci1yqoHkCKQZmg5FxyLNJbGFRzSOYeBjPa2S74gTLSQFiutp6+404PpJQ/GgWf/SeDK78fs/NnfVbJ9SZ7w9Ae8wZOBbpRO+Er5aMqcb9TXFyVmN8iR7FI7NQ7vHEdRt3kLYbCGvsgn7SDJY1VN5ZYom25Enrm2oU4uE4b0/paxN2Uv4S4f+gapmjpb1/d+89raHlrldcVzWAPYgyOVEmZZy8cQW2G3Sd+lgIxAz8oM5m74anim0qu6TF/A9f16Tc+90U3DFeTNl8XH0NzpCzbh5J8n/c/JWbnwRPiLwxiYrHXVXUbE6TFzjgYaQ9RhkIcYJ5DP7Kxa7hzJHAPXCMC5ayJQUOUdwTBEZGZccv+49Sk4qIshGRXqmXyg06KsCmtkhE/03DdLtpIU8aveGBWgjpaMdzW5qLTVf6w9vfmnltkEl0EGGSoEO1vNxqO/pp6hOf4Hh88mWptAwXXnQ8mSGZIfPprmv90Ri7PGrpacPH0tAZISWUXKqo3TWiXo4YPPxut+nXUVKSxOCYQI3tLffwsLmgtfcw2lWqr0pRXrg33p1Ia94z2IiF884Bbn3lHOjFgeJxhI3PYnSkQRPMYA9rBGCE57QTuwNwv3fDlJ0nN4eSY8acRbsCkRSqPX+3MkpzIwzfMO8n8=") << '\n';
  // std::cout << port_density("1.1.1.1","5678") << '\n';
  // std::cout << port_density("1.21.1.1","5678") << '\n';
  // std::cout << port_density("1.1.13.1","5678") << '\n';
  // std::cout << port_density("1.1.1.11","5678") << '\n';
  // std::cout << port_density("1.1.13.1","5679") << '\n';
  

  // std::cout <<  findTokens("fasfsadsadsAIzaSyAzY-BfodQLArSEp1JeWV0hlOLGiRLBR8Q") << '\n';

  //  int num;
  //  std::cin >> num;
   
  #ifdef DEBUG_MODE
    std::cout<<"Running in Debug mode"<<std::endl;
  #endif
  
  std::unique_ptr<Config> config;

  try
  { 
    std::vector <std::string> compilation_flags;
    if(argc > 1)
    {
      config = std::make_unique<Config>(Config::get_config(argv[1]));
    }
    else
    {
      config = std::make_unique<Config>(Config::get_config());
    }

    // Initialize the server.
    Server server(std::move(config));

    // Run the server until stopped.
    server.run();
  }
  catch (std::exception& e)
  {
    Logger::log(e.what(), Logger::LOG_LEVEL::FATAL);
  }
   return 0;
    
}