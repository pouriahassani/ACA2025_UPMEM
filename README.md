## Upmem environment setup
```bash
git clone https://github.com/pouriahassani/ACA2025_UPMEM.git
cd ACA2025_UPMEM
apptainer shell --bind $(pwd):/workspace -B /run /shares/ziti-
opt/software/cat/aca2025.sif
source /opt/ACA2025/upmem-2025.1.0-Linux-x86_64/upmem_env.sh
cd exercises
```