#include "conf.hpp"
#include "core.hpp"

std::string GetOptLong(int argc, char *argv[])
{
  std::string rc = "";
  int opt, option_index = 0;
  const char *optstring = ":c:";

  static struct option long_options[] = {
      {"config_path", required_argument, NULL, 'c'}, {0, 0, 0, 0}};

  while (true)
  {
    if (-1 == (opt = getopt_long(argc, argv, optstring, long_options,
                                 &option_index)))
    {
      break;
    }
    if ('c' == opt)
    {
      rc.assign(argv[optind - 1]);
    }
  }
  return rc;
}


bool readConf(std::string conf, 
              std::vector<NodeInfo> & p,
              NodeInfo & c,
              Mode & m)
{
  std::fstream conf_file;
  conf_file.open(conf); 
  if (!conf_file.is_open())
  {
    return false;
  }

  p.reserve(3);
  

  // read
  while (!conf_file.eof())
  {
    std::string buf;
    getline(conf_file, buf);
    // Annotate
    if ('!' == buf[0])
    {
      continue;
    }

    // get the mode
    if(buf.find("mode") != std::string::npos) {
      if(buf.find("coordinator") != std::string::npos) {
        m = MODE_C;
      } else if(buf.find("participant") != std::string::npos) {
        m = MODE_P;
      } else {
        m = MODE_INVALID;
      }
    } 
    
    else if(buf.find("coordinator_info") != std::string::npos) {
      
      size_t pos = buf.find("coordinator_info");
      buf = buf.substr(pos + 16);
      
      buf.erase(remove(buf.begin(), buf.end(), ' '), buf.end());

      pos = buf.find(":");
      if(pos == std::string::npos) {
        return false;
      }
      else {
        c.add = buf.substr(0, pos - 1);
        c.port = strtol((buf.substr(pos + 1)).c_str(), nullptr, 10);
      }
    } 
    
    else if(buf.find("participant_info") != std::string::npos) {
      NodeInfo tmp;
      size_t pos = buf.find("participant_info");
      buf = buf.substr(pos + 16);
      buf.erase(remove(buf.begin(), buf.end(), ' '), buf.end());

      pos = buf.find(":");
      if(pos == std::string::npos) {
        return false;
      }
      else {
        tmp.add = buf.substr(0, pos - 1);
        tmp.port = strtol((buf.substr(pos + 1)).c_str(), nullptr, 10);
        p.push_back(tmp);
      }
    }
  }

  return true;
}

void showInfo(std::vector<NodeInfo>  p,
              NodeInfo  c,
              Mode  m)
{
  if(m == MODE_C) {
    std::cout << "COORDINATOR" << std::endl;
  } else if ( m == MODE_P ) {
    std::cout << "PARTICIPANT" << std::endl;
  } else {
    std::cout << "INVALID" << std::endl;
  }

  std::cout << "C: " << c.add << ":" << c.port << std::endl;

  for(auto entry : p) 
  {
    std::cout << "P: " << entry.add << ":" << entry.port << std::endl;
  }
}