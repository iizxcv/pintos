FROM --platform=linux/amd64 ubuntu:22.04

ENV TZ=Asia/Seoul
ENV LANG=ko_KR.UTF-8 LANGUAGE=ko_KR.UTF-8 LC_ALL=ko_KR.UTF-8
ENV DEBIAN_FRONTEND=noninteractive

# 기본 패키지 설치 + 로케일 생성
RUN apt-get update && apt-get install -y \
    locales tzdata build-essential gcc gdb vim git sudo qemu-system-x86 python3 \
    zsh curl wget unzip fonts-powerline doxygen less \
    && locale-gen ko_KR.UTF-8 \
    && update-locale LANG=ko_KR.UTF-8 \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# jungle 사용자 생성 및 sudo 권한 부여
RUN useradd -m -s /bin/zsh jungle && echo "jungle ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/jungle

USER jungle
WORKDIR /home/jungle

# oh-my-zsh 설치 (비대화식)
RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended

# powerlevel10k 테마 설치
RUN git clone --depth=1 https://github.com/romkatv/powerlevel10k.git ~/.oh-my-zsh/custom/themes/powerlevel10k

# 플러그인 설치
RUN git clone https://github.com/zsh-users/zsh-autosuggestions ~/.oh-my-zsh/custom/plugins/zsh-autosuggestions && \
    git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ~/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting

# .zshrc 수정: 테마, 플러그인 설정
RUN sed -i 's/^ZSH_THEME=.*/ZSH_THEME="powerlevel10k\/powerlevel10k"/' ~/.zshrc && \
    sed -i 's/^plugins=(git)/plugins=(git zsh-autosuggestions zsh-syntax-highlighting)/' ~/.zshrc

# jungle 사용자의 기본 쉘을 zsh로 설정 (docker exec 등에서 자동 zsh)
RUN sudo chsh -s /bin/zsh jungle

# bashrc에 추가 명령 (필요시 경로 조정)
RUN echo "source /workspaces/pintos/pintos/activate" >> ~/.bashrc

CMD ["zsh"]
