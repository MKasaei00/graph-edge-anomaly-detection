make clean
make

if [ "$1" == "cic-unsw-nb15-DoS" ]; then

#  echo "Preprocessing Graph Labels according to time_window and edge_threshold"
#  python3 process_data.py DARPA 30 50

  echo "Running AnoEdge-G"
  # Algorithm => anoedge_g
  # Dataset => cic-unsw-nb15-DoS
  # Rows => 2
  # Buckets => 9
  # Decay factor => 0.9
  ./main anoedge_g "$1" 2 9 0.9

  echo "Running AnoEdge-L"
  # Algorithm => anoedge_g
  # Dataset => cic-unsw-nb15-DoS
  # Rows => 2
  # Buckets => 9
  # Decay factor => 0.9
  ./main anoedge_l "$1" 2 9 0.9

  echo "Installing python dependencies"
  pip3 install -r requirements.txt -q

  echo "Running python metrics"
  python3 metrics.py --dataset "$1" --time_window 30 --edge_threshold 50

fi

if [ "$1" == "DARPA" ]; then

#  echo "Preprocessing Graph Labels according to time_window and edge_threshold"
#  python3 process_data.py DARPA 30 50

  echo "Running AnoEdge-G"
  # Algorithm => anoedge_g
  # Dataset => DARPA
  # Rows => 2
  # Buckets => 32
  # Decay factor => 0.9
  ./main anoedge_g DARPA 2 32 0.9

#  echo "Running AnoEdge-L"
  # Algorithm => anoedge_g
  # Dataset => DARPA
  # Rows => 2
  # Buckets => 32
  # Decay factor => 0.9
#  ./main anoedge_l DARPA 2 32 0.9

  echo "Installing python dependencies"
  ~/envs/local/bin/pip install -r requirements.txt -q

  echo "Running python metrics"
  ~/envs/local/bin/python metrics.py --dataset DARPA --time_window 30 --edge_threshold 50

fi

if [ "$1" == "ISCX" ]; then

  echo "Preprocessing Graph Labels according to time_window and edge_threshold"
  python3 process_data.py ISCX 60 100

  echo "Running AnoEdge-G"
  ./main anoedge_g ISCX 2 32 0.9

  echo "Running AnoEdge-L"
  ./main anoedge_l ISCX 2 32 0.9

  python3 metrics.py --dataset ISCX --time_window 60 --edge_threshold 100

fi

if [ "$1" == "EdgeAnomalyEPlusPlus" ]; then

#  echo "Preprocessing Graph Labels according to time_window and edge_threshold"
#  python3 process_data.py DARPA 30 50

  echo "Running AnoEdge-G"
  # Algorithm => anoedge_g
  # Dataset => DARPA
  # Rows => 2
  # Buckets => 32
  # Decay factor => 0.9
  ./main anoedge_g EdgeAnomalyEPlusPlus 2 32 0.9

  echo "Running AnoEdge-L"
  # Algorithm => anoedge_g
  # Dataset => DARPA
  # Rows => 2
  # Buckets => 32
  # Decay factor => 0.9
  ./main anoedge_l EdgeAnomalyEPlusPlus 2 32 0.9

  echo "Installing python dependencies"
  pip3 install -r requirements.txt -q

  echo "Running python metrics"
  python3 metrics.py --dataset EdgeAnomalyEPlusPlus --time_window 30 --edge_threshold 50

fi
