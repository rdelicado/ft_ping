# Diagrama de Flujo - ft_ping

```mermaid
flowchart TD
    A["Inicio: leer argumentos"] --> B["Resolver hostname<br/>(getaddrinfo)"]
    B --> C["Crear socket raw"]
    C --> D["Configurar socket:<br/>timeout y opciones"]
    D --> E["Construir paquete<br/>ICMP Echo Request"]
    E --> F["Obtener timestamp<br/>(gettimeofday)"]
    F --> G["Enviar el paquete<br/>(sendto)"]
    G --> H["Esperar respuesta<br/>(recvfrom)"]
    H --> I{"¿Respuesta recibida?"}
    I -->|No| J["Mostrar timeout"]
    I -->|Sí| K["Calcular RTT,<br/>mostrar resultados"]
    J --> L{"¿Continuar enviando?"}
    K --> L
    L -->|Sí| E
    L -->|No| M["Mostrar estadísticas<br/>y salir"]
    M --> N["Fin"]
    
    style A fill:#2d5f7f,stroke:#5da9e9,stroke-width:3px,color:#fff
    style B fill:#3d4f5f,stroke:#7da9e9,stroke-width:2px,color:#fff
    style C fill:#3d4f5f,stroke:#7da9e9,stroke-width:2px,color:#fff
    style D fill:#3d4f5f,stroke:#7da9e9,stroke-width:2px,color:#fff
    style E fill:#4f5f3d,stroke:#a9e97d,stroke-width:2px,color:#fff
    style F fill:#4f5f3d,stroke:#a9e97d,stroke-width:2px,color:#fff
    style G fill:#4f5f3d,stroke:#a9e97d,stroke-width:2px,color:#fff
    style H fill:#5f3d4f,stroke:#e97da9,stroke-width:2px,color:#fff
    style I fill:#7f6d2d,stroke:#e9c95d,stroke-width:3px,color:#fff
    style J fill:#5f4f3d,stroke:#e9b97d,stroke-width:2px,color:#fff
    style K fill:#3d5f4f,stroke:#7de9a9,stroke-width:2px,color:#fff
    style L fill:#7f6d2d,stroke:#e9c95d,stroke-width:3px,color:#fff
    style M fill:#5f3d3d,stroke:#e97d7d,stroke-width:2px,color:#fff
    style N fill:#7f2d2d,stroke:#e95d5d,stroke-width:3px,color:#fff
```

## Visualización

Para ver este diagrama correctamente:
- **VS Code**: Instala la extensión "Markdown Preview Mermaid Support" y abre la vista previa (Ctrl+Shift+V)
- **GitHub**: Se renderiza automáticamente
