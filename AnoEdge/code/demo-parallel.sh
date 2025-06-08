make clean
make

if [ "$1" == "DARPA" ]; then

  # Clean state and remove files if required
  rm -rf ../data/${1}_*
  rm -rf ../results/anoedge_g_${1}_*_time.csv
  rm -rf ../results/anoedge_g_${1}_*_score.csv
  rm -rf ../results/anoedge_g_${1}_score.csv

  # Partition data into p files for labels and data
  ~/envs/local/bin/python split.py ../data/$1 Data.csv $2
  ~/envs/local/bin/python split.py ../data/$1 Label.csv $2
  
  # run algorithm for every item
  echo "Running AnoEdge-G"
  # Algorithm => anoedge_g
  # Dataset => DARPA
  # Rows => 2
  # Buckets => 32
  # Decay factor => 0.9
  for i in $(seq 1 $2); do
    id=$(printf "DARPA_%03d" "$i")
    ./main anoedge_g "$id" 2 32 0.9
    echo "$id run completed"
  done

  # Concat all the files in a way that can be done
  ~/envs/local/bin/python recombine.py ../results $1 $2

  # Install dependencies
  echo "Installing python dependencies"
  ~/envs/local/bin/pip install -r requirements.txt -q

  # evaluate metrics AUCROC
  ~/envs/local/bin/python metrics.py --dataset DARPA --time_window 30 --edge_threshold 50
fi
