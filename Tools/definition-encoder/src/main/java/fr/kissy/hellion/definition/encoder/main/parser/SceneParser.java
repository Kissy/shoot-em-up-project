package fr.kissy.hellion.definition.encoder.main.parser;

import com.google.protobuf.ByteString;
import fr.kissy.hellion.definition.encoder.main.utils.AssertUtils;
import fr.kissy.hellion.definition.encoder.main.utils.ParseUtils;
import fr.kissy.hellion.proto.Common;
import fr.kissy.hellion.proto.Definition;
import org.apache.commons.lang3.StringUtils;
import org.bson.types.ObjectId;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.management.modelmbean.XMLParseException;
import javax.xml.parsers.DocumentBuilderFactory;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id: SceneParser.java 32 2012-03-29 14:44:19Z kissy $
 */
public class SceneParser extends AbstractParser {
    
    private boolean include = false;
    private List<Common.SystemType> systems;
    private List<String> objects = new ArrayList<>();

    /**
     * @inheritDoc
     */
    public SceneParser(String xmlPath, String outputPath, List<Common.SystemType> systems) throws XMLParseException {
        super(xmlPath, outputPath);
        this.systems = systems;
    }

    /**
     * @inheritDoc
     */
    public SceneParser(String xmlPath, String outputPath, List<Common.SystemType> systems, boolean include) throws XMLParseException {
        this(xmlPath, outputPath, systems);
        this.include = include;
    }

    /**
     * @inheritDoc
     */
    @Override
    protected void parse() {
        try {
            System.out.println("Parsing SDF File " + xmlFile.getPath());

            builder = Definition.Scene.newBuilder();

            Element documentElement = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(xmlFile).getDocumentElement();
            documentElement.normalize();

            AssertUtils.makeTest("Scene".equals(documentElement.getNodeName()), "SDF files must contain Scene root element");
            
            if (include) {
                getSceneDefinitionBuilder().setName("Included");
            } else {
                parseScene(documentElement);
            }

            NodeList nodeList = documentElement.getChildNodes();
            for (int i = 0; i < nodeList.getLength(); i++) {
                Node node = nodeList.item(i);
                if (node.getNodeType() == Node.ELEMENT_NODE) {
                    if ("Properties".equals(node.getNodeName())) {
                        parseSystemProperties((Element) node);
                    } else if ("Objects".equals(node.getNodeName())) {
                        parseObjects((Element) node);
                    } else if ("Links".equals(node.getNodeName())) {
                        parseLinks((Element) node);
                    } else if ("Include".equals(node.getNodeName())) {
                        parseInclude((Element) node);
                    } else {
                        AssertUtils.makeTest(true, "SDF files must contain Properties, Objects and Links children only");
                    }
                }
            }

            System.out.println("SDF File parsed\n");
            parsed = true;
        } catch (Exception e) {
            throw new RuntimeException("Cannot parse the SDF File " + xmlFile.getPath() + ", " + e.getMessage());
        }
    }

    /**
     * Parse the include element.
     *
     * @param node The include node.
     * @throws javax.management.modelmbean.XMLParseException The exception.
     */
    private void parseInclude(Element node) throws XMLParseException {
        SceneParser includeParser = new SceneParser(xmlFile.getParent() + "\\" + ParseUtils.safeGetAttribute(node, "SDF") + ".sdf",
                outputFile.getAbsolutePath(), systems, true);
        Definition.Scene.Builder builder = (Definition.Scene.Builder) includeParser.getBuilder();
        // TODO make system properties update correctly
        getSceneDefinitionBuilder().addAllObjects(builder.getObjectsList());
        getSceneDefinitionBuilder().addAllLinks(builder.getLinksList());
    }

    /**
     * Parse the scene.
     * 
     * @param documentElement The document element.
     * @throws javax.management.modelmbean.XMLParseException The exception.
     */
    private void parseScene(Element documentElement) throws XMLParseException {
        System.out.println("\t- Parsing Scene");

        getSceneDefinitionBuilder().setName(ParseUtils.safeGetAttribute(documentElement, "Name"));
    }

    /**
     * Parse the resources.
     *
     * @param propertiesElement The Properties element.
     * @throws javax.management.modelmbean.XMLParseException Exception if the document is malformed.
     */
    private void parseSystemProperties(Element propertiesElement) throws XMLParseException {
        System.out.println("\t- Parsing Properties");

        Common.System.Builder systems = ParseUtils.parseSystemProperties(propertiesElement);
        AssertUtils.makeTest(this.systems.contains(systems.getType()), systems.getType() + " is not a known systemType");

        getSceneDefinitionBuilder().addSystems(systems);
    }

    /**
     * Parse the objects.
     *
     * @param objectsElement The Objects element.
     * @throws javax.management.modelmbean.XMLParseException Exception if the document is malformed.
     */
    private void parseObjects(Element objectsElement) throws XMLParseException {
        System.out.println("\t- Parsing Properties");

        // Store object properties (used because ODF / Object properties can be the same)
        Map<String, Common.SystemObject> propertiesMap = new HashMap<>();

        // Object properties
        NodeList objects = objectsElement.getElementsByTagName("Object");
        for (int i = 0; i < objects.getLength(); i++) {
            Common.Object.Builder objectBuilder = Common.Object.newBuilder();
            Element objectElement = (Element) objects.item(i);
            
            // Object name
            String id = objectElement.getAttribute("Id");
            objectBuilder.setId(StringUtils.isNotBlank(id) ? id : new ObjectId().toString());
            objectBuilder.setName(ParseUtils.safeGetAttribute(objectElement, "Name"));
            objectBuilder.setParent(objectElement.getAttribute("Parent"));

            // ODF : odf properties will be overridden by object properties
            String odf = objectElement.getAttribute("ODF");
            if (StringUtils.isNotEmpty(odf)) {
                ObjectParser objectParser = new ObjectParser(xmlFile.getParent() + "\\"  + odf + ".odf",
                        outputFile.getAbsolutePath(), objectBuilder);
                try {
                    objectParser.writeBuilder();
                } catch (IOException ignored) {}
                
                // Fill the properties map
                for (Common.SystemObject properties : objectBuilder.getSystemObjectsList()) {
                    propertiesMap.put(properties.getType(), properties);
                }
            }
    
            // Object properties
            NodeList properties = objectElement.getElementsByTagName("Properties");
            for (int j = 0; j < properties.getLength(); j++) {
                Common.SystemObject.Builder propertiesBuilder = ParseUtils.parseObjectProperties((Element) properties.item(j));
                if (propertiesMap.containsKey(propertiesBuilder.getType())) {
                    List<String> currentSystemProperties = getCurrentSystemProperties(propertiesBuilder.getPropertiesList());
                    for (Common.Property property : propertiesBuilder.getPropertiesList()) {
                        if (currentSystemProperties.contains(property.getName())) {
                            updateProperty(propertiesBuilder, property);
                        } else {
                            propertiesBuilder.addProperties(property);
                        }
                    }
                } else {
                    objectBuilder.addSystemObjects(propertiesBuilder);
                }
            }
            
            // Add the object to the list
            this.objects.add(objectBuilder.getName());
    
            getSceneDefinitionBuilder().addObjects(objectBuilder);
        }
    }

    /**
     * Remove the current property.
     * 
     * @param propertiesBuilder The property builder.
     * @param property The property.
     */
    private void updateProperty(Common.SystemObject.Builder propertiesBuilder, Common.Property property) {
        for (int i = 0; i < propertiesBuilder.getPropertiesCount(); i++) {
            if (property.getName().equals(propertiesBuilder.getProperties(i).getName())) {
                Common.Property.Builder propertyBuilder = propertiesBuilder.getProperties(i).toBuilder();
                propertyBuilder.clearValue();
                for (ByteString value : property.getValueList()) {
                    propertyBuilder.addValue(value);
                }
                break;
            }
        }
    }

    /**
     * Get the list of properties name.
     *
     * @param propertiesList The list of Property.
     * @return The list of Property names.
     */
    private List<String> getCurrentSystemProperties(List<Common.Property> propertiesList) {
        List<String> names = new ArrayList<>();
        for (Common.Property property : propertiesList) {
            names.add(property.getName());
        }
        return names;
    }

    /**
     * Parse the links.
     * 
     * @param linksElement The Links element.
     * @throws javax.management.modelmbean.XMLParseException The exception.
     */
    private void parseLinks(Element linksElement) throws XMLParseException {
        System.out.println("\t- Parsing Links");

        // Object properties
        NodeList links = linksElement.getElementsByTagName("Link");
        for (int i = 0; i < links.getLength(); i++) {
            Definition.Link.Builder linkBuilder = Definition.Link.newBuilder();
            Element linkElement = (Element) links.item(i);
            
            linkBuilder.setSubject(ParseUtils.safeGetAttribute(linkElement, "Subject"));
            linkBuilder.setObserver(ParseUtils.safeGetAttribute(linkElement, "Observer"));
            String subjectSystemType = ParseUtils.safeGetAttribute(linkElement, "SubjectSystem");
            linkBuilder.setSubjectSystemType(Common.SystemType.valueOf(subjectSystemType));
            String observerSystemType = ParseUtils.safeGetAttribute(linkElement, "ObserverSystem");
            linkBuilder.setObserverSystemType(Common.SystemType.valueOf(observerSystemType));

            AssertUtils.makeTest(objects.contains(linkBuilder.getSubject()),
                    linkBuilder.getSubject() + " is not a known object");
            AssertUtils.makeTest(objects.contains(linkBuilder.getObserver()),
                    linkBuilder.getObserver() + " is not a known object");
            AssertUtils.makeTest(systems.contains(linkBuilder.getSubjectSystemType()),
                    linkBuilder.getSubjectSystemType() + " is not a known systemType");
            AssertUtils.makeTest(systems.contains(linkBuilder.getObserverSystemType()),
                    linkBuilder.getObserverSystemType() + " is not a known systemType");
            
            getSceneDefinitionBuilder().addLinks(linkBuilder);
        }
    }

    /**
     * Get the SDF Builder.
     *
     * @return The SDF Builder.
     */
    public Definition.Scene.Builder getSceneDefinitionBuilder() {
        return (Definition.Scene.Builder) builder;
    }
}
