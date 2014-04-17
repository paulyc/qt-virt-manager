#include "create_virt_domain.h"

/*
 * http://libvirt.org/formatdomain.htm
 *
    General metadata
    Operating system booting
        BIOS bootloader
        Host bootloader
        Direct kernel boot
        Container boot
    SMBIOS System Information
    CPU Allocation
    CPU Tuning
    Memory Allocation
    Memory Backing
    Memory Tuning
    NUMA Node Tuning
    Block I/O Tuning
    Resource partitioning
    CPU model and topology
    Events configuration
    Power Management
    Hypervisor features
    Time keeping
    Devices
        Hard drives, floppy disks, CDROMs
        Filesystems
        Device Addresses
        Controllers
        Device leases
        Host device assignment
            USB / PCI / SCSI devices
            Block / character devices
        Redirected devices
        Smartcard devices
        Network interfaces
            Virtual network
            Bridge to LAN
            Userspace SLIRP stack
            Generic ethernet connection
            Direct attachment to physical interface
            PCI Passthrough
            Multicast tunnel
            TCP tunnel
            Setting the NIC model
            Setting NIC driver-specific options
            Overriding the target element
            Specifying boot order
            Interface ROM BIOS configuration
            Quality of service
            Setting VLAN tag (on supported network types only)
            Modifying virtual link state
        Input devices
        Hub devices
        Graphical framebuffers
        Video devices
        Consoles, serial, parallel & channel devices
            Guest interface
                Parallel port
                Serial port
                Console
                Channel
            Host interface
                Domain logfile
                Device logfile
                Virtual console
                Null device
                Pseudo TTY
                Host device proxy
                Named pipe
                TCP client/server
                UDP network console
                UNIX domain socket client/server
                Spice channel
        Sound devices
        Watchdog device
        Memory balloon device
        Random number generator device
        TPM device
        NVRAM device
        panic device
    Security label

*/

CreateVirtDomain::CreateVirtDomain(QWidget *parent, QString str) :
    QDialog(parent), type(str)
{
    setModal(true);
    setWindowTitle("Domain Settings");
    restoreGeometry(settings.value("DomCreateGeometry").toByteArray());
    commonLayout = new QVBoxLayout(this);
    general = new General(this, type);
    boot = new QWidget(this);
    cpu = new QWidget(this);
    memory = new QWidget(this);
    power = new QWidget(this);
    devices = new QWidget(this);
    interfaces = new QWidget(this);
    security = new QWidget(this);
    tabWidget = new QTabWidget(this);
    tabWidget->addTab(general, QIcon::fromTheme("domain"), "General");
    tabWidget->addTab(boot, QIcon::fromTheme("domain"), "Boot");
    tabWidget->addTab(cpu, QIcon::fromTheme("domain"), "CPU");
    tabWidget->addTab(memory, QIcon::fromTheme("domain"), "Memory");
    tabWidget->addTab(power, QIcon::fromTheme("domain"), "Power");
    tabWidget->addTab(devices, QIcon::fromTheme("domain"), "Devices");
    tabWidget->addTab(interfaces, QIcon::fromTheme("domain"), "Interfaces");
    tabWidget->addTab(security, QIcon::fromTheme("domain"), "Security");
    ok = new QPushButton("Ok", this);
    ok->setAutoDefault(true);
    connect(ok, SIGNAL(clicked()), this, SLOT(set_Result()));
    cancel = new QPushButton("Cancel", this);
    cancel->setAutoDefault(true);
    connect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(ok);
    buttonLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);
    commonLayout->addWidget(tabWidget);
    commonLayout->addWidget(buttons);
    setLayout(commonLayout);
    xml = new QTemporaryFile(this);
    xml->setAutoRemove(false);
    xml->setFileTemplate(QString("%1%2XML_Desc-XXXXXX.xml").arg(QDir::tempPath()).arg(QDir::separator()));
}
CreateVirtDomain::~CreateVirtDomain()
{
    settings.setValue("DomCreateGeometry", saveGeometry());
    disconnect(ok, SIGNAL(clicked()), this, SLOT(set_Result()));
    disconnect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
    delete general;
    general = 0;
    delete boot;
    boot = 0;
    delete cpu;
    cpu = 0;
    delete memory;
    memory = 0;
    delete power;
    power = 0;
    delete devices;
    devices = 0;
    delete interfaces;
    interfaces = 0;
    delete security;
    security = 0;
    delete tabWidget;
    tabWidget = 0;
    delete ok;
    ok = 0;
    delete cancel;
    cancel = 0;
    delete buttonLayout;
    buttonLayout = 0;
    delete buttons;
    buttons = 0;
    delete commonLayout;
    commonLayout = 0;
    delete xml;
    xml = 0;
}

/* public slots */
QString CreateVirtDomain::getXMLDescFileName() const
{
    return xml->fileName();
}

/* private slots */
void CreateVirtDomain::buildXMLDescription()
{
    QDomDocument doc = QDomDocument();
    QDomElement root, elem;
    root = doc.createElement("domain");
    root.setAttribute("type", type.toLower());
    doc.appendChild(root);
    QDomNodeList list = general->getNodeList();
    /*
     * current DomNode is removed to root-elrment
     * but NULL-elemens not removed
     * therefore keep to seek on not-NULL next element
     */
    uint j = 0;
    uint count = list.length();
    for (uint i=0; i<=count;i++) {
        //qDebug()<<list.item(j).nodeName()<<i;
        if (!list.item(j).isNull()) root.appendChild(list.item(j));
        else ++j;
    };
    //qDebug()<<doc.toString();

    bool read = xml->open();
    if (read) xml->write(doc.toByteArray(4).data());
    xml->close();
}
void CreateVirtDomain::set_Result()
{
    if ( sender()==ok ) {
        setResult(QDialog::Accepted);
        buildXMLDescription();
    } else {
        setResult(QDialog::Rejected);
    };
    done(result());
}