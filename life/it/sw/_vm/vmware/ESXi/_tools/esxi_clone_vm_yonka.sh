
# example: 
#   `sh {name}.sh ubuntu-20.04.1 7 8 9` to gen ubuntu-20.04.1_7, ubuntu-20.04.1_8, ubuntu-20.04.1_9
#   `OUT_DIR=../sata-1t sh {name}.sh ubuntu-20.04.1 10 11 12` to ... to specified out dir

# cd to parent dir of base_dir
base_dir="$1"
shift
of_types="vmx vmxf nvram"
function do_clone() {
  local i="$1"

  local cp_dir=${base_dir}_$i
  local cp_dir_path=$cp_dir
  if [[ -n "$OUT_DIR" ]]; then
    local out_dir_prefix=${OUT_DIR}/
    cp_dir_path="$OUT_DIR/$cp_dir"
  fi

  echo "clone $base_dir to $cp_dir_path"
  mkdir ${cp_dir_path}

  # -flat.vmdk格式的由命令生成: -00001.vmdk格式的是snap，按需获取
  for dkf in `ls ${base_dir}/*.vmdk | grep -v \\\\-flat.vmdk`; do
    # 下面命令，前提是path里没有空行
    vmkfstools -i ${dkf} -d thin "${out_dir_prefix}${dkf//${base_dir}/${cp_dir}}"
  done

  for ext in ${of_types}; do
    for f in `ls ${base_dir}/*.${ext}`; do
      local dst_f="${out_dir_prefix}${f//${base_dir}/${cp_dir}}"
      cp "$f" "${dst_f}"
      sed -i "s/${base_dir}/${cp_dir}/g" "${dst_f}"
    done
  done
}

for name in "$@"; do
  do_clone "${name}"
done

