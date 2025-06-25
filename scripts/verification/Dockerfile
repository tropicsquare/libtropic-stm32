# Dockerfile
FROM python:3-slim-bookworm

RUN apt-get update && apt-get install -y git build-essential cmake gcc-arm-none-eabi openocd

# Copy the entrypoint script
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]