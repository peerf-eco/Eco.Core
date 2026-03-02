package Eco.Annotations;

import javax.annotation.processing.*;
import javax.lang.model.SourceVersion;
import javax.lang.model.element.*;
import javax.lang.model.type.TypeMirror;
import javax.tools.Diagnostic;
import javax.tools.JavaFileObject;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;
import java.util.Set;

@SupportedAnnotationTypes("Eco.Annotations.EcoNative")
@SupportedSourceVersion(SourceVersion.RELEASE_8)
public class EcoNativeProcessor extends AbstractProcessor {
    private static final String BASE_CLASS = "IEcoUnknownNative";
    private static final String BASE_INTERFACE = "IEcoUnknown";

    @Override
    public boolean process(Set<? extends TypeElement> annotations, RoundEnvironment roundEnv) {
        for (Element element : roundEnv.getElementsAnnotatedWith(EcoNative.class)) {
            if (element.getKind() != ElementKind.INTERFACE) {
                processingEnv.getMessager().printMessage(Diagnostic.Kind.ERROR, "@EcoNative can only be applied to the interface", element);
                continue;
            }
            TypeElement interfaceElement = (TypeElement) element;
            if (!extendsEcoUnknown(interfaceElement)) {
                processingEnv.getMessager().printMessage(Diagnostic.Kind.ERROR, "An interface with @EcoNative must extend the interface " + BASE_INTERFACE, element);
                continue;
            }
            try {
                generateClass(interfaceElement);
            } catch (IOException e) {
                processingEnv.getMessager().printMessage(Diagnostic.Kind.ERROR, "Native class generation error: " + e.getMessage(), element);
            }
        }
        return true;
    }

    private boolean extendsEcoUnknown(TypeElement interfaceElement) {
        for (TypeMirror directInterface : interfaceElement.getInterfaces()) {
            if (directInterface.toString().equals(BASE_INTERFACE) ||
                    directInterface.toString().endsWith("." + BASE_INTERFACE)) {
                return true;
            }
            Element ifaceElement = processingEnv.getTypeUtils().asElement(directInterface);
            if (ifaceElement instanceof TypeElement && extendsEcoUnknown((TypeElement) ifaceElement)) {
                return true;
            }
        }
        return false;
    }

    private void generateClass(TypeElement interfaceElement) throws IOException {
        String interfaceName = interfaceElement.getSimpleName().toString();
        String generatedClassName = interfaceName + "Native";
        String packageName = processingEnv.getElementUtils().getPackageOf(interfaceElement).getQualifiedName().toString();
        JavaFileObject builderFile = processingEnv.getFiler().createSourceFile(packageName + "." + generatedClassName, interfaceElement);
        try (PrintWriter out = new PrintWriter(builderFile.openWriter())) {
            if (!packageName.isEmpty()) {
                out.println("package " + packageName + ";");
                out.println();
            }
            out.println("import Eco.Core." + BASE_CLASS + ";");
            out.println();
            out.println("public class " + generatedClassName + " extends " + BASE_CLASS +
                    " implements " + interfaceName + " {");
            out.println();
            List<? extends Element> members = interfaceElement.getEnclosedElements();
            int methodIndex = 3;
            for (Element member : members) {
                if (member.getKind() == ElementKind.METHOD) {
                    ExecutableElement method = (ExecutableElement) member;
                    if (method.getModifiers().contains(Modifier.STATIC) || method.getModifiers().contains(Modifier.DEFAULT)) {
                        continue;
                    }
                    generateMethod(out, method, methodIndex);
                    methodIndex++;
                }
            }
            out.println("}");
        }
    }

    private void generateMethod(PrintWriter out, ExecutableElement method, int index) {
        out.println("    @Override");
        out.print("    public ");
        String returnType = method.getReturnType().toString();
        out.print(returnType + " ");
        out.print(method.getSimpleName() + "(");
        List<? extends VariableElement> params = method.getParameters();
        for (int i = 0; i < params.size(); i++) {
            VariableElement param = params.get(i);
            out.print(param.asType() + " " + param.getSimpleName());
            if (i < params.size() - 1) {
                out.print(", ");
            }
        }
        out.println(") {");
        out.print("        ");
        if (!returnType.equals("void")) {
            out.print("return (" + boxIfPrimitive(returnType) + ") ");
        }
        out.print("invokeNative(" + index + ", new Object[]{");
        for (int i = 0; i < params.size(); i++) {
            VariableElement param = params.get(i);
            out.print(param.getSimpleName());
            if (i < params.size() - 1) {
                out.print(", ");
            }
        }
        out.println("}, " + returnType + ".class);");
        out.println("    }");
        out.println();
    }

    private String boxIfPrimitive(String type) {
        switch (type) {
            case "byte":    return "Byte";
            case "short":   return "Short";
            case "int":     return "Integer";
            case "long":    return "Long";
            case "float":   return "Float";
            case "double":  return "Double";
            case "char":    return "Character";
            case "boolean": return "Boolean";
            default:        return type;
        }
    }
}