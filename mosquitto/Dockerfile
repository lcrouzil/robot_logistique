FROM eclipse-mosquitto

COPY ./mosquitto.conf /mosquitto/config/mosquitto.conf
COPY ./entrypoint.sh /

ENTRYPOINT ["sh", "/entrypoint.sh"]

CMD ["/usr/sbin/mosquitto", "-c", "/mosquitto/config/mosquitto.conf"]