sim_window = {
  view = {
    simulator_view = true;
  };
fullscreen = false;
};

sim_window = {
  view = {
    simulator_view = true;
  };
fullscreen = false;
};

perspective_window = {
  view = {
    eyes = {
      eye = cavr.sixdof("vrpn[ShortGlasses[0]]");
      --left_eye = cavr.sixdof("emulated3");
      --right_eye = cavr.sixdof("emulated2");
      --stereo ="mono";
    };
    lower_left = cavr.sixdof("vrpn[ShortGlasses[0]]") * cavr.translate(-1, -1, -1);
    lower_right = cavr.sixdof("vrpn[ShortGlasses[0]]") * cavr.translate(1, -1, -1);
    upper_left = cavr.sixdof("vrpn[ShortGlasses[0]]") * cavr.translate(-1, 1, -1);
  };
  fullscreen = false;
};

x11_renderer = {
  type = "x11gl";
  display = ":1.1";
  windows = {
    sw = perspective_window;
    --sim_window2 = sim_window;

  };
};

x11_renderer2 = {
  type = "x11gl";
  display = ":1.2";
  windows = {
    sw = perspective_window;
    --sim_window2 = sim_window;

  };
};

x11_renderer3 = {
  type = "x11gl";
  display = ":2.0";
  windows = {
    sw = perspective_window;
    sim_window = sim_window;
  };
};


x11_renderer4 = {
  type = "x11gl";
  display = ":0.0";
  windows = {
    sw = perspective_window;
    --sim_window2 = sim_window;
  };
};


vrpn = {
  type = "vrpn";
  input_name = "vrpn";
  buttons = {
    --"Button0@localhost";
    "WiiMote0@projector.cse.unr.edu";
  };
  analogs = {
    "WiiMote0@projector.cse.unr.edu";
  };
  sixdofs = {
    "ShortGlasses@tracker.rd.unr.edu:1";
    "WiiMote0@tracker.rd.unr.edu";
    --"WiiMote1@tracker.rd.unr.edu";
  };
};

self = {
  hostname = HOSTNAME;
  ssh = HOSTNAME;--"chase@" .. HOSTNAME;
  address = HOSTNAME;
  plugins = {
    --x11_renderer = x11_renderer;
    x11_renderer3 = x11_renderer3;
    vrpn = vrpn;
  };
};


others = {
  hostname = "hpcvis4";
  ssh = "hpcvis4.cse.unr.edu";
  address = "hpcvis4.cse.unr.edu";--"tcp://" .. "hpcvis7" .. ":8888";
  plugins = {
    x11_renderer4 = x11_renderer4;
    --x11_renderer2 = x11_renderer2;
  };
};

others2 = {
  hostname = "hpcvis2";
  ssh = "hpcvis2";
  address = "hpcvis2";--"tcp://" .. "hpcvis7" .. ":8888";
  plugins = {
    x11_renderer = x11_renderer;
    vrpn = vrpn;
  };
};


machines = {
  self=self;
  --self2 = others;
  --self3 = others2;
};
